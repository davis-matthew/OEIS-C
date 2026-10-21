/*
 * A228313  Triangle read by rows: T(p,q) (1<=q<=p) is the Wiener index of the
 *          Cartesian product of the cycles C(p) and C(q) (the torus grid
 *          graph).
 * https://oeis.org/A228313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(p,q) where p is the smallest integer such that p(p+1)/2
 *            ≥ n, q = n - (p-1)p/2, and T(p,q) is the Wiener index of
 *            C(p)×C(q) given by the four parity cases in the OEIS entry
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find row p via binary search: p(p+1)/2 ≥ n */
    idx_t low = 1, high = n;               /* high is a safe upper bound */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t val = mid * (mid + 1) / 2;  /* mid*(mid+1)/2 */
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t p = low;                            /* 1‑based row */
    idx_t q = n - (p - 1) * p / 2;            /* column within the row */

    /* Convert to seq_t for arithmetic */
    seq_t P = p;
    seq_t Q = q;
    seq_t result;

    /* Compute T(p,q) according to parity of p and q */
    if ((p & 1U) && (q & 1U)) {                     /* both odd */
        result = P * Q * (P + Q) * (P * Q - 1) / 8;
    } else if (!(p & 1U) && !(q & 1U)) {            /* both even */
        result = P * P * Q * Q * (P + Q) / 8;
    } else if ((p & 1U) && !(q & 1U)) {             /* p odd, q even */
        result = P * Q * Q * (P * P - 1 + P * Q) / 8;
    } else {                                       /* p even, q odd */
        result = P * P * Q * (Q * Q - 1 + P * Q) / 8;
    }

    return result;
}
