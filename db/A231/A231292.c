/*
 * A231292  a(n) = Jacobsthal(n)^n, where Jacobsthal(n) = A001045(n), for
 *          n>=1.
 * https://oeis.org/A231292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - (-1)^n)^n / 3^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Jacobsthal(n) is computed by J(1)=1, J(2)=1 and J(k)=J(k-1)+2*J(k-2) */
/* a(n) = J(n)^n.  All arithmetic is performed in the built seq_t type. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* value undefined for n < 1 */
    if (n == 1 || n == 2)
        return 1;              /* J(1)=J(2)=1 ⇒ a(n)=1 */

    /* Compute Jacobsthal(n) via the recurrence */
    seq_t J_prev = 1;          /* J(1) */
    seq_t J_curr = 1;          /* J(2) */
    seq_t J_next;
    for (idx_t i = 3; i <= n; ++i) {
        J_next = J_curr + (J_prev << 1);   /* J(i) = J(i-1) + 2*J(i-2) */
        J_prev = J_curr;
        J_curr = J_next;
    }

    /* Fast exponentiation: J(n)^n   (O(log n) multiplications) */
    seq_t result = 1;
    seq_t base   = J_curr;
    seq_t exp    = n;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
