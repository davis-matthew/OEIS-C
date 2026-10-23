/*
 * A184517  Upper s-Wythoff sequence, where s=4n-2. Complement of A184516.
 * https://oeis.org/A184517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((2*n-1)*phi^2)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 3124, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   phi = (1+sqrt(5))/2,  phi^2 = (3+sqrt(5))/2    */

static inline seq_t
isqrt_seq(seq_t x)
{
    /* Binary search for floor(sqrt(x)) for arbitrary-precision seq_t. */
    if (x == 0)
        return 0;
    seq_t low  = 0;
    seq_t high = x;                 /* high^2 >= x for all x >= 0 */

    while (low <= high) {
        seq_t mid  = (low + high) / 2;
        seq_t mid2 = mid * mid;
        if (mid2 == x)
            return mid;
        else if (mid2 < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                     /* high is floor(sqrt(x))  */
}

seq_t a(idx_t n)
{
    /* k = 2*n - 1  (intentionally cast to seq_t for integer arithmetic) */
    seq_t k = (seq_t)n * 2 - 1;

    /* floor(k*sqrt(5)) == floor(sqrt(5*k*k)) */
    seq_t prod        = k * k;
    prod             *= (seq_t)5;
    seq_t floorKsqrt5 = isqrt_seq(prod);

    /* ceil(k*(3+sqrt(5))/2) = floor((3k + floor(k*sqrt5))/2) + 1  */
    seq_t numerator   = (seq_t)3 * k + floorKsqrt5;
    seq_t result      = numerator / 2 + 1;

    return result;
}
