/*
 * A049626  a(n) = T(n,4), array T as in A049615.
 * https://oeis.org/A049626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 + 14 * floor((n-1)/6) + p[(n-1) mod 6] where p = {0, 3,
 *            5, 8, 9, 13} for n ≥ 0. This implements A049626
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;          /* a(0) = 3 */

    /* compute sum of increments from k=2 to n */
    idx_t t = n - 1;                      /* number of added terms */
    idx_t q = t / 6;                      /* complete 6‑step cycles */
    idx_t r = t % 6;                      /* remaining steps in the last cycle */

    seq_t sum = (seq_t)14 * (seq_t)q;      /* 14 per full cycle */

    /* add partial sum for the remaining steps */
    switch (r) {
        case 0: break;                     /* 0 */
        case 1: sum += (seq_t)3; break;    /* 3 */
        case 2: sum += (seq_t)5; break;    /* 3 + 2 */
        case 3: sum += (seq_t)8; break;    /* 3 + 2 + 3 */
        case 4: sum += (seq_t)9; break;    /* +1 */
        case 5: sum += (seq_t)13; break;   /* +4 */
    }

    return (seq_t)3 + sum;                /* a(n) = initial value + sum of increments */
}
