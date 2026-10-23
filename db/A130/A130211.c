/*
 * A130211  Triangle read by rows: matrix product A054522 * A000012.
 * https://oeis.org/A130211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum_{d | r, d ≥ j} φ(d),
 * where r = smallest integer with r(r+1)/2 ≥ n
 * and j = n − (r−1)r/2.
 * This follows from A130211 = A054522 * A000012 with
 * T(n,k) = sum_{d|n, d≥k} φ(d).
 */

seq_t a(idx_t n)
{
    /* pre–compute φ(k) for k ≤ 20000 (enough for n ≤ 10000) */
    static seq_t phi[20001];
    static int init = 0;

    if (!init) {
        for (idx_t i = 0; i <= 20000; ++i)
            phi[i] = (seq_t)i;          /* φ(i) := i initially           */
        phi[0] = 0;
        phi[1] = 1;
        for (idx_t p = 2; p <= 20000; ++p) {
            if (phi[p] == (seq_t)p) {   /* p is prime */
                for (idx_t j = p; j <= 20000; j += p)
                    phi[j] -= phi[j] / (seq_t)p;
            }
        }
        init = 1;
    }

    /* find r with r(r+1)/2 ≥ n */
    idx_t r = 1;
    while (((seq_t)r * (r + 1)) / 2 < (seq_t)n)
        ++r;

    /* previous triangular number */
    seq_t prevTri = ((seq_t)(r - 1) * r) / 2;

    /* column index j = n − prevTri   (j fits in idx_t) */
    idx_t j = (idx_t)(((seq_t)n) - prevTri);

    /* sum φ(d) over divisors d of r that are ≥ j */
    seq_t sum = 0;
    for (idx_t d = j; d <= r; ++d)
        if (r % d == 0)
            sum += phi[d];

    return sum;
}
