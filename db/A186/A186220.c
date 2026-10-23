/*
 * A186220  Adjusted joint rank sequence of (g(i)) and (f(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          squares. Complement of A186219.
 * https://oeis.org/A186220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A186220(a, n) = rank of n² in the merged list of triangular
 *            numbers T_k = k(k+1)/2 and squares S_k = k², with triangular
 *            numbers preceding squares when equal. For each n ≥ 1: a(n) = n +
 *            (number of triangular numbers < n²) + (1 if n² is triangular
 *            else 0)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t floor_sqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;         /* upper mid to avoid infinite loop */
        if (mid > 0 && x / mid < mid)             /* mid*mid > x, check via division to avoid overflow */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1, return 0 for safety */
        return 0;

    seq_t m = (seq_t)n * (seq_t)n;            /* m = n² */

    /* Count squares strictly less than m: all squares for k = 1 .. n-1 */
    seq_t s_less = (seq_t)(n - 1);

    /* Count triangular numbers strictly less than m.
       Solve k(k+1)/2 ≤ m-1  =>  k = floor((sqrt(1+8*(m-1))-1)/2) */
    seq_t target_minus1 = m - 1;
    seq_t tmp = (seq_t)8 * target_minus1 + 1;   /* 1 + 8*(m-1) */
    seq_t sqrt_less = floor_sqrt_seq(tmp);
    seq_t t_less = (sqrt_less - 1) / 2;

    /* Test whether m itself is triangular: 1 + 8*m must be a perfect square. */
    seq_t delta = (seq_t)8 * m + 1;             /* 1 + 8*m */
    seq_t sqrt_delta = floor_sqrt_seq(delta);
    seq_t triangular = (sqrt_delta * sqrt_delta == delta) ? 1 : 0;

    return (seq_t)n + t_less + triangular;
}
