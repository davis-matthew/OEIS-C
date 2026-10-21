/*
 * A062170  Maximum value of factorials mod n.
 * https://oeis.org/A062170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{k>=1} (k! mod n) (offset 1: a(1)=0)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the factorial iteratively until it becomes 0 modulo n.  
 * The maximum value among all factorials modulo n is returned.  */
seq_t a(idx_t n)
{
    if (n == 0)                /* safety for invalid call, but n>=1 in tests */
        return 0;

    seq_t fact = 1 % (seq_t)n; /* 1! mod n */
    seq_t result = fact;       /* current maximum */

    for (idx_t k = 1; k < n; ++k) {
        fact = (fact * (seq_t)k) % (seq_t)n;
        if (fact > result)
            result = fact;
        if (fact == 0)   /* subsequent factorials will stay 0 */
            break;
    }
    return result;
}
