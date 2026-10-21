/*
 * A220627  Prime numbers that are not Brazilian.
 * https://oeis.org/A220627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 6470, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime that is NOT a Brazilian prime,
   i.e., not a repunit in any base b ≥ 2 with ≥ 3 digits.  The sequence
   is defined as A220627.  The function builds primes on demand and
   discards those that are Brazilian, caching the results. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cache of all primes found so far */
static seq_t *prime_tbl = NULL;
static size_t prime_tbl_cap = 0;
static size_t prime_tbl_cnt = 0;

/* Cache of the sequence values a(n) */
static seq_t *seq_tbl     = NULL;
static size_t seq_tbl_cap = 0;
static size_t seq_tbl_cnt = 0;

/* Ensure the prime cache has at least `min_cap` slots. */
static void ensure_primes_cap(size_t min_cap)
{
    if (min_cap <= prime_tbl_cap) return;
    size_t new_cap = prime_tbl_cap ? prime_tbl_cap * 2 : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(prime_tbl, new_cap * sizeof(seq_t));
    prime_tbl = tmp;
    prime_tbl_cap = new_cap;
}

/* Ensure the sequence cache has at least `min_cap` slots. */
static void ensure_seq_cap(size_t min_cap)
{
    if (min_cap <= seq_tbl_cap) return;
    size_t new_cap = seq_tbl_cap ? seq_tbl_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_tbl, new_cap * sizeof(seq_t));
    seq_tbl = tmp;
    seq_tbl_cap = new_cap;
}

/* Compute a repunit value 1 + b + b^2 + … + b^(m-1).
   If the value exceeds `limit`, return limit+1 to indicate “too big”. */
static seq_t repunit_value(seq_t b, seq_t m, seq_t limit)
{
    seq_t val = 1;
    seq_t i = 1;
    while (i < m) {
        /* Prevent overflow: if val * b would be > limit, break. */
        if (val > limit / b) return limit + 1;
        val = val * b + 1;
        if (val > limit) return limit + 1;
        ++i;
    }
    return val;
}

/* Does a prime `p` equal a repunit of length `m` in some base? */
static bool repunit_exists(seq_t p, seq_t m)
{
    seq_t b = 2;
    while (true) {
        seq_t val = repunit_value(b, m, p);
        if (val == p) return true;
        if (val > p) return false;
        ++b;
    }
}

/* Return true iff `p` is a Brazilian prime. */
static bool is_brazilian_prime(seq_t p)
{
    seq_t pm1 = p - 1;
    seq_t d = 2;
    while (d * d <= pm1) {
        if (pm1 % d == 0) {
            seq_t m1 = d, m2 = pm1 / d;
            if (m1 >= 3 && repunit_exists(p, m1)) return true;
            if (m2 >= 3 && m2 != m1 && repunit_exists(p, m2)) return true;
        }
        ++d;
    }
    if (pm1 >= 3 && repunit_exists(p, pm1)) return true;
    return false;
}

/* Generate the next prime, add it to the cache, and, if it is not
   Brazilian, append it to the sequence cache.  The global candidate
   value is advanced to the next integer after a prime has been
   processed. */
static void generate_next_prime(void)
{
    static seq_t candidate = 2;
    while (true) {
        bool prime = true;
        for (size_t i = 0; i < prime_tbl_cnt; ++i) {
            seq_t p = prime_tbl[i];
            if (p > candidate / p) break;           /* p*p > candidate */
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            ensure_primes_cap(prime_tbl_cnt + 1);
            prime_tbl[prime_tbl_cnt++] = candidate;

            if (!is_brazilian_prime(candidate)) {
                ensure_seq_cap(seq_tbl_cnt + 1);
                seq_tbl[seq_tbl_cnt++] = candidate;
            }
            ++candidate;
            return;
        }
        ++candidate;
    }
}

/* Public function: return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, harmless */
    while (seq_tbl_cnt < n) generate_next_prime();
    return seq_tbl[n - 1];
}
