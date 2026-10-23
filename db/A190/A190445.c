/*
 * A190445  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(golden ratio,4,1) and
 *          []=floor.
 * https://oeis.org/A190445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4n+1)*phi) - 4*floor(n*phi) - floor(phi)
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 136, u64 n <= 136,
 *            u128 n <= 136, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where phi = (1+sqrt(5))/2.  The implementation uses only integer
   arithmetic.  For each integer k the value floor(k*sqrt(5)) is
   obtained as floor(sqrt(5*k*k)).  Square roots are computed by
   binary search avoiding overflow.  Only seq_t and idx_t are used. */

static seq_t integer_sqrt(seq_t x)
{
    if (x == 0)
        return 0;
    seq_t lo = 0;
    seq_t hi = x;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return hi;
}

static seq_t floor_sqrt5_mul(seq_t k)
{
    seq_t prod = k * k;
    prod = prod * (seq_t)5;
    return integer_sqrt(prod);
}

seq_t a(idx_t n)
{
    const seq_t b = (seq_t)4;
    const seq_t c = (seq_t)1;
    seq_t N = (seq_t)n;
    seq_t K = b * N + c;

    /* floor(K*phi) = K + floor((floor(K*sqrt5) - K)/2) */
    seq_t floor_K_sqrt5 = floor_sqrt5_mul(K);
    seq_t floor_K_phi = K + (floor_K_sqrt5 - K) / 2;

    /* floor(n*phi) = N + floor((floor(N*sqrt5) - N)/2) */
    seq_t floor_N_sqrt5 = floor_sqrt5_mul(N);
    seq_t floor_N_phi = N + (floor_N_sqrt5 - N) / 2;

    /* floor(c*phi) with c = 1 */
    seq_t floor_c_sqrt5 = floor_sqrt5_mul(c);
    seq_t floor_cr = c + (floor_c_sqrt5 - c) / 2; /* equals 1 */

    return floor_K_phi - b * floor_N_phi - floor_cr;
}
