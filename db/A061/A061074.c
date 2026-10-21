/*
 * A061074  Smallest number that begins with 1, has digits in order
 *          123...901... and is divisible by n. If no such number exists then
 *          a(n) = 0.
 * https://oeis.org/A061074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest number that begins with 1, has digits in
 *            the repeating order 1,2,3,4,5,6,7,8,9,0,1,... and is divisible
 *            by n. If no such number exists, the function returns 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* safety check; offset starts at 1 */
        return (seq_t)0;

    /* Digit pattern of length 10 */
    static const unsigned char pattern[10] = {1,2,3,4,5,6,7,8,9,0};

    idx_t  pos       = 0;                    /* current position in pattern */
    seq_t  rem       = (seq_t)0;            /* current remainder modulo n */
    seq_t  val       = (seq_t)0;            /* current constructed number */
    seq_t  n_seq     = (seq_t)n;            /* n as seq_t for arithmetic */

    /* visited[rem][pos] -> linear index (rem*10+pos)  */
    size_t vis_size = (size_t)n * 10;
    unsigned char *visited = NULL;
    if (vis_size != 0) {
        visited = (unsigned char *)calloc(vis_size, (size_t)1);
        if (!visited)  /* allocation failure -> no solution */
            return (seq_t)0;
    }

    while (1) {
        unsigned char digit = pattern[pos];

        /* Update remainder and value with the new digit */
        rem = (rem * (seq_t)10 + (seq_t)digit) % n_seq;
        val = val * (seq_t)10 + (seq_t)digit;

        if (rem == (seq_t)0) {
            if (visited)
                free(visited);
            return val;          /* found the smallest divisible number */
        }

        /* Advance to the next digit position */
        pos = (pos + 1) % 10;

        /* Compute linear index for the state (rem, pos) */
        idx_t rem_idx = (idx_t)rem;         /* rem < n, safe cast */
        size_t idx = (size_t)rem_idx * 10  + (size_t)pos;

        if (visited[idx]) {                /* state repeated – no solution */
            free(visited);
            return (seq_t)0;
        }
        visited[idx] = 1;
    }
}
