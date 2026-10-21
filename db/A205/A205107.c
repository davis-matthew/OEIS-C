/*
 * A205107  The least index j < k such that n divides s(k) - s(j), where s(m)
 *          = 3^m - 2^m = A001047(m) and k = A205000(n) is the least index for
 *          which such j exists.
 * https://oeis.org/A205107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = the minimal index j<k such that n divides s(k)-s(j),
 *            where s(m)=3^m-2^m. Computed by finding the first repeated
 *            residue of s(m) modulo n; the first occurrence index of that
 *            residue is returned
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is always ≥1 in the OEIS sequence */
    if (n == 0) return 0;                 /* defensive guard */

    /* array to hold the first occurrence of each residue (0 = unseen) */
    idx_t *first = (idx_t *)calloc(n, sizeof(idx_t));
    if (!first) return 0;                 /* allocation failure */

    idx_t p3 = 3 % n;                     /* 3^m mod n */
    idx_t p2 = 2 % n;                     /* 2^m mod n */
    idx_t answer = 0;

    for (idx_t m = 1; ; ++m) {
        /* residue r_m = (3^m - 2^m) mod n */
        idx_t r = (p3 + n - p2) % n;

        if (first[r]) {                   /* residue already seen */
            answer = first[r];
            break;
        }
        first[r] = m;                     /* record first occurrence */
        p3 = (p3 * 3) % n;                /* advance 3^m+1 */
        p2 = (p2 * 2) % n;                /* advance 2^m+1 */
    }

    free(first);
    return (seq_t)answer;
}
