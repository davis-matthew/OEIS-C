/*
 * A018763  Divisors of 992.
 * https://oeis.org/A018763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 992 in increasing order
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: full, pass, 12 terms through n = 12 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute and cache the 12 divisors of 992 in increasing order.
   992 = 2^5 * 31.  There are (5+1)*(1+1) = 12 divisors. */
static const seq_t *divisors_of_992(void)
{
    /* Static array to hold the sorted divisors.  It is initialized
       once on first call. */
    static seq_t divs[12];
    static int initialized = 0;
    if (initialized) {
        return divs;
    }

    /* Merge 2^i (i=0..5) and 31*2^j (j=0..5) while keeping order. */
    int i = 0;          /* exponent of 2 in the first set        */
    int j = 0;          /* exponent of 2 in the second set       */
    int k = 0;          /* position in the result array          */

    while (i <= 5 && j <= 5) {
        seq_t val_i = ((seq_t)1 << i);            /* 2^i          */
        seq_t val_j = ((seq_t)31 << j);          /* 31*2^j       */
        if (val_i < val_j) {
            divs[k++] = val_i;
            i++;
        } else {
            divs[k++] = val_j;
            j++;
        }
    }
    /* Remaining elements from the first set */
    while (i <= 5) {
        divs[k++] = ((seq_t)1 << i);
        i++;
    }
    /* Remaining elements from the second set */
    while (j <= 5) {
        divs[k++] = ((seq_t)31 << j);
        j++;
    }

    initialized = 1;
    return divs;
}

/* Sequence A018763: divisors of 992 in increasing order.
   Off‑by‑one: a(1) is the first divisor. */
seq_t a(idx_t n)
{
    /* The sequence has only 12 terms; return 0 for n > 12. */
    const seq_t *divs = divisors_of_992();
    if (n == 0 || n > 12) {
        return 0;
    }
    return divs[n - 1];
}
