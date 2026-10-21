/*
 * A066301  a(n) = 0 if n is squarefree, otherwise 1 + a(n/rad(n)) where rad =
 *          A007947 (squarefree kernel).
 * https://oeis.org/A066301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is squarefree, otherwise 1 + a(n/rad(n)) where rad
 *            = A007947 (squarefree kernel)
 * Range:     u8 n <= 7784, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/rad.h"
#include "../utils/is_squarefree.h"

#define MAX_CACHE 10000   /* pre‑cache values for n <= 10 000 */

static seq_t cache[MAX_CACHE + 1];
static bool cached[MAX_CACHE + 1] = { false };

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == 1) return 0;
    if (n <= MAX_CACHE && cached[n]) return cache[n];

    /* Compute using the definition */
    seq_t steps = 0;
    idx_t cur = n;
    while (!is_squarefree((seq_t)cur)) {
        steps++;
        /* rad(cur) divides cur, and rad(cur) >= 2 */
        seq_t rad_cur = rad(cur);
        cur = cur / (idx_t)rad_cur;
    }

    /* Store in cache if within bounds */
    if (n <= MAX_CACHE) {
        cache[n] = steps;
        cached[n] = true;
    }
    return steps;
}
