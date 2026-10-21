/*
 * A085696  a(n) = L(n) * L(n+1) * L(n+2) / 2, where L(n) = Lucas number
 *          (A000032).
 * https://oeis.org/A085696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = L(n) * L(n+1) * L(n+2) / 2 where L(n) is the Lucas number
 *            (A000032). One of L(n), L(n+1), L(n+2) is even, so we divide
 *            that one by 2 first to avoid overflow in 64‑bit builds
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of Lucas numbers */
    static seq_t *lucas = NULL;
    static idx_t lucas_max = 0;  /* highest index computed */

    /* initialise cache with L(0)=2, L(1)=1, L(2)=3 */
    if (lucas == NULL) {
        lucas = (seq_t *)malloc(sizeof(seq_t) * 3);
        if (lucas == NULL) exit(1);
        lucas[0] = (seq_t)2;
        lucas[1] = (seq_t)1;
        lucas[2] = (seq_t)3;
        lucas_max = 2;
    }

    /* ensure cache contains L(n+2) */
    idx_t need = n + 2;
    if (need > lucas_max) {
        size_t new_size = (size_t)(need + 1);
        seq_t *tmp = (seq_t *)realloc(lucas, new_size * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        lucas = tmp;
        for (idx_t i = lucas_max + 1; i <= need; ++i)
            lucas[i] = lucas[i - 1] + lucas[i - 2];
        lucas_max = need;
    }

    seq_t l0 = lucas[n];
    seq_t l1 = lucas[n + 1];
    seq_t l2 = lucas[n + 2];

    /* divide the even factor by 2 to keep intermediate values small */
    if ((l0 & (seq_t)1) == 0)
        l0 >>= 1;
    else if ((l1 & (seq_t)1) == 0)
        l1 >>= 1;
    else
        l2 >>= 1;

    seq_t result = l0;
    result *= l1;
    result *= l2;
    return result;
}
