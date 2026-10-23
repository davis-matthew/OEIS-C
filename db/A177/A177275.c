/*
 * A177275  Primes which are a concatenation of some permutation of the first
 *          5 primes.
 * https://oeis.org/A177275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177275: primes which are a concatenation of a permutation of the
   first five primes [2,3,5,7,11].  All 5! = 120 permutations are
   enumerated, the intermediate value is built on the fly, tested for
   primality (using the provided is_prime_big helper), sorted, and the
   nth element is returned.  Indices start at 1 as in OEIS. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Comparator for qsort on seq_t values. */
static int cmp_seq(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Recursive generation of concatenations. */
static void gen_perm(const seq_t primes[5], const seq_t mult[5],
                     uint8_t used[5], size_t pos, seq_t cur,
                     seq_t out[], size_t *cnt)
{
    if (pos == 5) {
        if (is_prime_big(cur)) {
            out[*cnt] = cur;
            ++*cnt;
        }
        return;
    }
    for (size_t i = 0; i < 5; ++i) {
        if (!used[i]) {
            used[i] = 1;
            seq_t nxt = cur * mult[i] + primes[i];
            gen_perm(primes, mult, used, pos + 1, nxt, out, cnt);
            used[i] = 0;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* One-time generation of all terms. */
    static seq_t all_vals[120];
    static size_t total = 0;
    static bool ready = false;

    if (!ready) {
        const seq_t primes[5] = {2, 3, 5, 7, 11};
        /* 10^digits of each prime: 10 for 1‑digit, 100 for 2‑digit (11). */
        const seq_t mult[5] = {10, 10, 10, 10, 100};
        uint8_t used[5] = {0};

        gen_perm(primes, mult, used, 0, 0, all_vals, &total);
        /* Sort ascending to provide a(1), a(2), … */
        qsort(all_vals, total, sizeof(seq_t), cmp_seq);
        ready = true;
    }

    if (n == 0) return 0;                 /* undefined for n < 1 */
    size_t idx = (size_t)(n - 1);
    if (idx >= total) return 0;            /* sequence ends after 20 terms */
    return all_vals[idx];
}
