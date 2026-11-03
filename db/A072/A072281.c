/*
 * A072281  Numbers n such that phi(n) + 1 and phi(n) - 1 are twin primes.
 * https://oeis.org/A072281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 5050, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9885 (2026-11-03)
 * Helpers:   is_prime_u64, mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A072281: numbers n such that phi(n)-1 and phi(n)+1 are twin primes.
 * The function generates values on demand, caching results for future calls.
 * It keeps a search that increments n until enough terms are found.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/totient_seq.h"
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

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t last_checked = 0;   /* highest n examined so far */

    while (len < n) {
        /* Examine the next candidate n */
        last_checked++;
        /* Compute φ(n) */
        seq_t phi_val = totient_seq((seq_t)last_checked);

        /* φ(n) < 3 → φ(n)-1 < 2, so not a twin‑prime pair */
        if (phi_val < 3)
            continue;

        seq_t low  = phi_val - 1;
        seq_t high = phi_val + 1;

        /* Both φ(n)-1 and φ(n)+1 must be prime */
        if (is_prime_big(low) && is_prime_big(high)) {
            /* Append to the cache */
            if (len == cap) {
                cap = cap == 0 ? 8 : cap * 2;
                terms = mem_resize(seq_t, terms, cap);
            }
            terms[len++] = (seq_t)last_checked;
        }
    }

    return terms[n - 1];
}
