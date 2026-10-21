/*
 * A211288  a(n) = number of n-lettered words in the alphabet {1, 2, 3} with
 *          as many occurrences of the substring (consecutive subword) [1, 1,
 *          2] as of [1, 1, 3].
 * https://oeis.org/A211288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n‑letter words over {1,2,3} with as many 112 as
 *            113. * The values are computed by a dynamic programme that keeps
 *            track of the last * two letters and the difference between the
 *            number of occurrences of 112 * and 113. There are only three
 *            relevant classes for the last two letters:...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 20000               /* should be ≥ maximum n requested */
#define SIZE (2 * MAX_N + 1)      /* array size accommodating all possible differences */

static seq_t cur0[SIZE], cur1[SIZE], cur2[SIZE];
static seq_t next0[SIZE], next1[SIZE], next2[SIZE];

/* Return the nth term of OEIS A211288.  a(0)=1, a(1)=3, a(2)=9, … */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    const size_t offset   = (size_t)n;          /* index of difference 0 */
    const size_t size_np1 = 2 * (size_t)n + 1;  /* number of indices actually used */

    /* pointers to the current and next state arrays */
    seq_t *C0 = cur0, *C1 = cur1, *C2 = cur2;
    seq_t *N0 = next0, *N1 = next1, *N2 = next2;

    /* initialise current arrays to zero in the used interval */
    for (size_t i = 0; i < size_np1; ++i) {
        C0[i] = C1[i] = C2[i] = 0;
    }

    /* start with the 2‑letter words */
    C0[offset] = (seq_t)1;     /* (1,1) */
    C1[offset] = (seq_t)2;     /* (2,1) and (3,1) */
    C2[offset] = (seq_t)6;     /* all other pairs */

    /* iterate for each additional letter, from length 3 up to n */
    for (idx_t len = 3; len <= n; ++len) {
        size_t min_cur = offset - (size_t)(len-1);
        size_t max_cur = offset + (size_t)(len-1);

        /* zero the next arrays over the range that will actually be written */
        size_t min_next = offset - (size_t)len;
        size_t max_next = offset + (size_t)len;
        for (size_t i = min_next; i <= max_next; ++i) {
            N0[i] = N1[i] = N2[i] = 0;
        }

        /* transition from every current state */
        for (size_t i = min_cur; i <= max_cur; ++i) {
            seq_t v0 = C0[i];
            if (v0) {
                N0[i]   += v0;     /* add letter '1' → stay in category 0 */
                N2[i+1] += v0;     /* add letter '2' → diff +1, go to cat 2 */
                N2[i-1] += v0;     /* add letter '3' → diff –1, go to cat 2 */
            }
            seq_t v1 = C1[i];
            if (v1) {
                N0[i]   += v1;     /* add '1' → diff unchanged, cat 0 */
                N2[i]   += v1;     /* add '2' → diff unchanged, cat 2 */
                N2[i]   += v1;     /* add '3' → diff unchanged, cat 2 */
            }
            seq_t v2 = C2[i];
            if (v2) {
                N1[i]   += v2;     /* add '1' → diff unchanged, cat 1 */
                N2[i]   += v2;     /* add '2' → diff unchanged, cat 2 */
                N2[i]   += v2;     /* add '3' → diff unchanged, cat 2 */
            }
        }

        /* swap current and next for the next iteration */
        seq_t *tmp;
        tmp = C0; C0 = N0; N0 = tmp;
        tmp = C1; C1 = N1; N1 = tmp;
        tmp = C2; C2 = N2; N2 = tmp;
    }

    /* the answer is the sum of all states with difference zero */
    return C0[offset] + C1[offset] + C2[offset];
}
