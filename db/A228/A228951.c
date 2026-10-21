/*
 * A228951  Numbers k such that decimal expansion of k^k has only one zero.
 * https://oeis.org/A228951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   Compute by brute‑force search of k such that the decimal *
 *            expansion of k^k contains exactly one zero digit. The sequence *
 *            is finite, so all terms are generated once and cached
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The sequence values (the k’s themselves).  They all fit into
 * seq_t for the expected range. */
static seq_t *terms = NULL;
static size_t terms_len = 0;

/* Compute k^k in base 10 using digit array and count zeros. */
static void compute_terms(void)
{
    const idx_t LIMIT = 200;                 /* search limit */
    const size_t DIGITS_CAP = 2000;          /* max digits needed up to LIMIT */
    int digits[DIGITS_CAP];

    size_t cap = 32;
    terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!terms) exit(1);

    for (idx_t k = 1; k <= LIMIT; ++k) {
        if (k % 10 == 0) continue;          /* skip multiples of 10 (many trailing zeros) */

        /* initialise value = 1 */
        size_t len = 1;
        digits[0] = 1;

        /* multiply by k, k times */
        for (idx_t i = 0; i < k; ++i) {
            long long carry = 0;
            for (size_t d = 0; d < len; ++d) {
                long long prod = (long long)digits[d] * k + carry;
                digits[d] = (int)(prod % 10);
                carry      = prod / 10;
            }
            while (carry) {
                digits[len++] = (int)(carry % 10);
                carry /= 10;
            }
        }

        /* count zeros in the decimal expansion */
        int zero_count = 0;
        for (size_t d = 0; d < len; ++d)
            if (digits[d] == 0) ++zero_count;

        if (zero_count == 1) {
            if (terms_len == cap) {
                cap *= 2;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!terms) exit(1);
            }
            terms[terms_len++] = (seq_t)k;
        }
    }
}

seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        compute_terms();
        initialized = 1;
    }
    if (n == 0 || n > terms_len)
        return (seq_t)0;           /* undefined for out‑of‑range indices */
    return terms[n - 1];
}
