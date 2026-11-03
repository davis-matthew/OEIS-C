/*
 * A237288  Lexicographically earliest sequence of noncomposite numbers such
 *          that a(n)*n / sum(i=1..n, a(n) ) is strictly increasing.
 * https://oeis.org/A237288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the lexicographically earliest sequence of non‑composite
 *            numbers (1 and primes) such that rₙ = n·a(n) / Σ_{i=1..n} a(i)
 *            is strictly increasing. Computed by a greedy search: for each
 *            n>1 pick the smallest prime greater than the floor of
 *            ((n-1)*a(n-1)*S) / (n*S-(n-1)*a(n-1)), where S = Σ_{i...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 43, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* static cache of computed terms: terms[0] = a(1) ... terms[sz-1] = a(sz) */
    static seq_t *terms = NULL;
    static size_t sz = 0;          /* number of terms stored              */
    static seq_t cum_sum = 0;      /* Σ_{i=1..sz} a(i)                    */

    /* offset: a(1) = 1 */
    if (n == 0) return 0;   /* outside offset – not used but safe */

    if (sz == 0) {
        terms = mem_new(seq_t, 1);
        if (!terms) exit(1);
        terms[0] = (seq_t)1;   /* a(1) */
        sz = 1;
        cum_sum = (seq_t)1;
    }

    if (n <= sz) return terms[n-1];

    for (idx_t k = (idx_t)(sz + 1); k <= n; ++k) {
        seq_t prev = terms[sz - 1];   /* a(k-1)           */
        seq_t S    = cum_sum;         /* Σ_{i=1}^{k-1}a(i) */

        seq_t numerator   = (seq_t)(k-1) * prev * S;
        seq_t denominator = (seq_t)k * S - (seq_t)(k-1) * prev;

        seq_t threshold = numerator / denominator;   /* floor division   */
        seq_t candidate = threshold + (seq_t)1;
        if (candidate < (seq_t)2) candidate = (seq_t)2;   /* primes start at 2 */

        while (!is_prime_big(candidate))
            candidate += (seq_t)1;

        /* append the candidate */
        terms = mem_resize(seq_t, terms, (sz + 1));
        if (!terms) exit(1);
        terms[sz] = candidate;
        ++sz;
        cum_sum += candidate;
    }

    return terms[n-1];
}
