/*
 * A076425  Numbers n such that zero is never reached by iterating the mapping
 *          k -> abs(reverse(lpd(k))-reverse(gpf(k))). lpd(k) is the largest
 *          proper divisor and gpf(k) is the largest prime factor of k.
 * https://oeis.org/A076425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A076425
 * The sequence consists of the numbers n for which
 * iterating the mapping
 *   k → |reverse(largest proper divisor of k) – reverse(largest
 *      prime factor of k)|
 * never reaches 0.  The function builds the sequence on demand
 * by testing successive integers.  All arithmetic is performed
 * in 64‑bit unsigned integers before casting the final term to
 * seq_t, which allows arbitrary‑precision builds.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* reverse the decimal digits of a 64‑bit unsigned integer */
static uint64_t reverse_uint64(uint64_t x)
{
    uint64_t rev = 0;
    while (x != 0) {
        rev = rev * 10 + (x % 10);
        x   /= 10;
    }
    return rev;
}

/* compute next value of the mapping k → |rev(lpd(k)) – rev(gpf(k))|  */
static uint64_t next_val(uint64_t k)
{
    if (k == 0)
        return 0;

    uint64_t n   = k;
    uint64_t spf = 0;   /* smallest prime factor */
    uint64_t gpf = 1;   /* largest prime factor */

    /* factorisation by trial division */
    if (n % 2 == 0) {
        spf = 2;
        while (n % 2 == 0)
            n /= 2;
        gpf = 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            if (spf == 0)
                spf = p;
            while (n % p == 0)
                n /= p;
            if (p > gpf)
                gpf = p;
        }
    }
    if (n > 1) {          /* remainder is prime */
        if (spf == 0)
            spf = n;
        if (n > gpf)
            gpf = n;
    }
    if (spf == 0)          /* k == 1 */
        spf = 1;

    uint64_t lpd = k / spf;   /* largest proper divisor */
    uint64_t rev_lpd = reverse_uint64(lpd);
    uint64_t rev_gpf = reverse_uint64(gpf);

    return rev_lpd > rev_gpf ? rev_lpd - rev_gpf : rev_gpf - rev_lpd;
}

/* test whether the orbit of k ever reaches 0 */
static bool ok(uint64_t k)
{
    uint64_t slow = k;
    uint64_t fast = k;

    while (1) {
        slow = next_val(slow);
        if (slow == 0)
            return false;

        fast = next_val(fast);
        if (fast == 0)
            return false;
        fast = next_val(fast);
        if (fast == 0)
            return false;

        if (slow == fast)   /* cycle found that does not contain 0 */
            return true;
    }
}

/* storage for computed terms */
static seq_t *terms   = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static uint64_t last_checked = 1;

/* main interface: return the nth term of A076425 (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1 – return 0 */

    if (terms_len < (size_t)n) {
        if (terms == NULL) {
            terms_cap = 128;
            terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        }
        while (terms_len < (size_t)n) {
            ++last_checked;
            if (ok(last_checked)) {
                if (terms_len == terms_cap) {
                    terms_cap <<= 1;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = (seq_t)last_checked;
            }
        }
    }
    return terms[n - 1];
}
