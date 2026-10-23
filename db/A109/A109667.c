/*
 * A109667  Beginning with 1, a(n) = n times the digits of concatenation of
 *          a(1),a(2), ...a(n-1) read backwards.
 * https://oeis.org/A109667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1; for n>1, a(n) = n*rev(concat_{i=1}^{n-1} a(i))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   pow10_of, rev_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10_of.h"
#include "../utils/rev_num.h"

/* return the number of decimal digits of x */
static idx_t digit_count(seq_t x)
{
    idx_t c = 0;
    do {
        ++c;
        x /= 10;
    } while (x != 0);
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused, safety */

    static seq_t *terms = NULL;    /* terms[1] = a(1), etc. */
    static idx_t terms_cap = 0;
    static idx_t terms_len = 0;    /* highest index stored */

    /* maintain state for incremental computation */
    static seq_t rev_concat = 0;   /* rev(concat of a(1)..a(k-1)) for current k */
    static idx_t digits_rev = 0;  /* number of digits of rev_concat */
    static seq_t pow10 = 1;       /* 10^digits_rev */

    /* allocate terms array if first call */
    if (!terms) {
        terms_cap = 16;
        terms = (seq_t*)malloc(sizeof(seq_t) * terms_cap);
    }

    /* compute terms up to n if needed */
    for (idx_t k = terms_len + 1; k <= n; ++k) {
        if (k == 1) {
            terms[1] = 1;          /* a(1) = 1 */
        } else {
            /* rev(concat_{i=1}^{k-1} a(i)) = rev(a(k-1)) * 10^{digits_rev} + rev_concat */
            seq_t rev_prev = rev_num(terms[k-1]);
            idx_t d_prev   = digit_count(terms[k-1]);

            rev_concat      = rev_prev * pow10 + rev_concat;
            digits_rev      += d_prev;

            seq_t pow_fac = pow10_of(d_prev);
            pow10        *= pow_fac;

            /* a(k) = k * rev_concat */
            terms[k] = ((seq_t)k) * rev_concat;
        }

        if (k >= terms_cap) {
            terms_cap <<= 1;
            terms = (seq_t*)realloc(terms, sizeof(seq_t) * terms_cap);
        }
        terms_len = k;
    }

    return terms[n];
}
