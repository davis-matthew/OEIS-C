/*
 * A092733  Sum of two consecutive primes of the form 2^x+1 (including Fermat
 *          primes and 2).
 * https://oeis.org/A092733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = p_n + p_{n+1} where p_i is the i-th prime of the form
 *            2^x+1 * (including x = 0, which gives 2). For n≥1 up to 5 this
 *            gives * a(1)=5, a(2)=8, a(3)=22, a(4)=274, a(5)=65794. * Beyond
 *            n=5 the sequence is undefined – the function returns 0 in that
 *            case
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Known primes of the form 2^x + 1: 2, 3, 5, 17, 257, 65537. */
    static const seq_t primes[6] = {2, 3, 5, 17, 257, 65537};

    if (n == 0) return 0;           /* offset is 1; a(0) not defined */
    if (n >= 1 && n <= 5) {
        /* n corresponds to index n in 1‑based terms, so primes[n-1] and primes[n] */
        return primes[n - 1] + primes[n];
    }
    /* For n > 5 the sequence has no defined terms in the OEIS entry. */
    return 0;
}
