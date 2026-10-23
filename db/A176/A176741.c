/*
 * A176741  Fibonacci-Chebyshev sequence depending on rabbit sequence A005614.
 * https://oeis.org/A176741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + (-1)^(h(n)+1)*a(n-2) with a(0)=0, a(1)=1, and
 *            h(n)=A005614(n) (the rabbit / Fibonacci word sequence). The sign
 *            s(n)=(-1)^(h(n)+1) is +1 when the n‑th character of the
 *            Fibonacci word is '0' and -1 when it is '1'
 * Range:     u8 n <= 27, u16 n <= 51, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Allocate array to store the first n+1 bits of the Fibonacci word */
    size_t need = (size_t)(n + 1);
    uint8_t *bits = (uint8_t *)malloc(need);
    if (!bits) return 0;   /* allocation failure */

    /* Build the Fibonacci word using the recurrence:
       S0 = "0",   S1 = "01",  Sk = Sk-1 + Sk-2.  Stop when length > n. */
    size_t len_prev = 1;          /* length of S_{k-1} ("0")   */
    size_t len_curr = 2;          /* length of S_k   ("01")    */
    uint8_t *prev = (uint8_t *)malloc(len_prev);
    uint8_t *curr = (uint8_t *)malloc(len_curr);
    if (!prev || !curr) { free(bits); free(prev); free(curr); return 0; }

    prev[0] = 0;
    curr[0] = 0;  curr[1] = 1;

    while (len_curr <= n) {
        size_t newlen = len_curr + len_prev;
        uint8_t *tmp = (uint8_t *)malloc(newlen);
        if (!tmp) { free(bits); free(prev); free(curr); return 0; }

        memcpy(tmp, curr, len_curr);
        memcpy(tmp + len_curr, prev, len_prev);

        free(prev);              /* previous S_{k-1} is no longer needed */
        prev = curr;             /* shift: previous current becomes new prev */
        curr = tmp;              /* new word becomes current */
        len_prev = len_curr;
        len_curr = newlen;
    }

    /* Copy the first n+1 bits to the bits array */
    for (idx_t i = 0; i <= n; ++i)
        bits[i] = curr[i];

    free(prev);
    free(curr);

    /* Compute the sequence using the recurrence */
    seq_t a0 = 0;          /* a(n-2)   */
    seq_t a1 = 1;          /* a(n-1)   */
    seq_t an = 0;          /* a(n)     */

    for (idx_t i = 2; i <= n; ++i) {
        int s = (bits[i] == 0) ? +1 : -1;   /* sign from Fibonacci word */
        if (s == +1)
            an = a1 + a0;   /* a(n-1) + a(n-2) */
        else
            an = a1 - a0;   /* a(n-1) - a(n-2); a1 >= a0 for all n in the sequence */

        a0 = a1;           /* shift window */
        a1 = an;
    }

    free(bits);
    return an;
}
