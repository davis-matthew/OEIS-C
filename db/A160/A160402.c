/*
 * A160402  Primes made up of all distinct digits except 0 and 1.
 * https://oeis.org/A160402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3098, u64 n <= 3098, u128 n <= 3098,
 *            big any n
 * Validated: full, pass, 3098 terms through n = 3098 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generate all 8‑digit permutations of {2..9}, test each for
   primality, sort the primes ascending, and return the nth such prime
   (OEIS A160402). */

#include <stdbool.h>
#include <stdlib.h>

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

static void gen_rec(int depth, seq_t cur, bool *used,
                    const uint8_t digits[8], seq_t *prime_buf, size_t *cnt)
{
    if (depth == 8) {
        if (is_prime_big(cur))
            prime_buf[(*cnt)++] = cur;
        return;
    }
    for (int i = 0; i < 8; ++i) {
        if (!used[i]) {
            used[i] = true;
            gen_rec(depth + 1, cur * 10 + digits[i], used, digits,
                    prime_buf, cnt);
            used[i] = false;
        }
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* One‑time initialisation (static local data). */
    static bool initialized = false;
    static seq_t primes[40320];
    static size_t prime_cnt = 0;

    if (!initialized) {
        bool used[8] = {0};
        const uint8_t digits[8] = {2,3,4,5,6,7,8,9};

        /* Generate all permutations and keep only the primes. */
        gen_rec(0, 0, used, digits, primes, &prime_cnt);

        /* Sort the primes in ascending order. */
        mem_qsort(primes, prime_cnt, sizeof(seq_t), cmp_seq_t);

        initialized = true;
    }

    /* The sequence is finite; return 0 for out‑of‑range indices. */
    if (n == 0 || n > prime_cnt)
        return 0;

    return primes[(size_t)(n - 1)];   /* a(1) is the first stored prime */
}
