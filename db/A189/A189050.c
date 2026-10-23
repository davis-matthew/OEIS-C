/*
 * A189050  a(n) = if n even then P((n-2)/2)+P(n/2) otherwise
 *          3*P((n+1)/2)+P((n-1)/2) where P(i) are the Pell numbers (A000129).
 * https://oeis.org/A189050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n is even then P((n-2)/2)+P(n/2) else
 *            3*P((n+1)/2)+P((n-1)/2), * where P(k) are the Pell numbers
 *            (P(0)=0, P(1)=1). * Recurrence used in the code: a(1)=3, a(2)=1,
 *            * a(n)=2*a(n-2)+a(n-4) for n≥5
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 48, u64 n <= 100,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds a cache of Pell numbers up to the largest index
 * required for the current input n and then evaluates the sequence
 * formula directly.  Memory for the cache is allocated with realloc
 * so that successive calls can extend the cache without reallocation
 * of the whole array. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of Pell numbers */
    static seq_t *pell = NULL;
    static idx_t pell_len = 0;  /* number of elements stored, >=0 */

    if (n < 1)
        return (seq_t)0;        /* undefined for offset 1, harmless */

    /* Determine the maximum Pell index required for this n */
    idx_t max_p = (n % 2 == 0 ? n / 2 : (n + 1) / 2);

    /* Extend the Pell cache if necessary */
    if (pell_len <= max_p) {
        size_t new_len = (size_t)(max_p + 1);    /* indices 0..max_p */

        /* realloc works for both C and C++ when cast */
        seq_t *tmp = (seq_t *)realloc(pell, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;     /* allocation failure, return 0 */

        pell = tmp;

        /* Initialise base Pell numbers if cache was empty */
        if (pell_len == 0) {
            pell[0] = (seq_t)0;
            pell[1] = (seq_t)1;
        }

        /* Compute any new Pell numbers */
        idx_t start = (pell_len > 1 ? pell_len : 2);  /* start from 2 if needed */
        for (idx_t i = start; i < new_len; ++i) {
            pell[i] = (seq_t)2 * pell[i - 1] + pell[i - 2];
        }

        pell_len = new_len;
    }

    /* Evaluate a(n) from the cached Pell numbers */
    seq_t result;
    if ((n % 2) == 0) {          /* n even */
        idx_t i1 = (n - 2) / 2;
        idx_t i2 = n / 2;
        result = pell[i1] + pell[i2];
    } else {                     /* n odd */
        idx_t i1 = (n + 1) / 2;
        idx_t i2 = (n - 1) / 2;
        result = (seq_t)3 * pell[i1] + pell[i2];
    }

    return result;
}
