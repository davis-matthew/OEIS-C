/*
 * A049761  Triangular array T, read by rows: T(n,k) = n^3 mod k, for k = 1..n
 *          and n >= 1.
 * https://oeis.org/A049761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r^3 mod k where (r,k) is the unique pair such that * r >=
 *            1, 1 <= k <= r and the index n corresponds to the k-th element *
 *            in row r of the triangular array T(r,·) with T(r,k) = r^3 mod k.
 *            * Rows are concatenated: a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2),
 *            a(4)=T(3,1), …
 * Range:     u8 n <= 91, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */

    /* Find the row r with cumulative size >= n */
    idx_t r = 1;
    for (;;) {
        seq_t cum = (seq_t)r * (r + 1) / 2;   /* sum 1..r */
        if (cum >= n) break;
        ++r;
    }

    /* Number of terms before row r */
    seq_t prev = (seq_t)r * (r - 1) / 2;

    /* Column index k  (1 <= k <= r) */
    seq_t k_seq = (seq_t)n - prev;   /* k as seq_t first */
    idx_t k = (idx_t)k_seq;          /* then convert back */

    /* Compute r^3 mod k safely using idx_t arithmetic */
    idx_t rmod = r % k;             /* r mod k */
    idx_t part = (rmod * rmod) % k; /* (r mod k)^2 mod k */
    idx_t res  = (part * rmod) % k; /* (r mod k)^3 mod k */

    return (seq_t)res;
}
