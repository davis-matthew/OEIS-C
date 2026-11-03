/*
 * A152293  Primes of the form : (p-n)/(n+1)=prime and (n+1)*p+n=prime. n=3.
 * https://oeis.org/A152293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that (p-3)/4 and 4p+3 are prime (OEIS A152293, n = 3).
   The function returns the n‑th such prime (1‑based indexing). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static idx_t next_candidate = 2;

/* Append a value to the internal sequence array. */
static void seq_add(seq_t v) {
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 64;
        seq = mem_resize(seq_t, seq, seq_cap);
    }
    seq[seq_len++] = v;
}

/* Generate terms until we have at least n of them. */
static void ensure_sequence(idx_t n) {
    while (seq_len < n) {
        /* Find the next prime p >= next_candidate. */
        idx_t p = next_candidate;
        while (!is_prime_big((seq_t)p))
            ++p;

        /* Check the two derived primes. */
        if (p >= 3 && ((p - 3) & 3) == 0) {
            idx_t q = (p - 3) >> 2;        /* (p-3)/4 */
            if (is_prime_big((seq_t)q)) {
                seq_t r = (seq_t)4 * (seq_t)p + (seq_t)3; /* 4p+3 */
                if (is_prime_big(r))
                    seq_add((seq_t)p);
            }
        }

        /* Advance to the next possible prime. */
        next_candidate = p + 1;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety for out of bounds, offset is 1 */
    ensure_sequence(n);
    return seq[n - 1];
}
