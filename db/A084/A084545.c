/*
 * A084545  Alternate number system in base 5.
 * https://oeis.org/A084545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{j=0..m-1} (1 + floor((4*n+1-5^m)/(4*5^j)) mod
 *            5)*10^j, * where m = floor(log₅(4*n+1)). This implements OEIS
 *            A084545
 * Range:     u8 n <= 30, u16 n <= 3905, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is offset 1; undefined for n==0 */

    /* K = 4*n + 1 */
    seq_t K = (seq_t)4 * n + 1;

    /* find m and p = 5^m such that 5^m <= K < 5^(m+1) */
    seq_t p = 1;          /* 5^m */
    seq_t next = 5;       /* candidate for 5^(m+1) */
    idx_t m = 0;
    while (next <= K) {
        p = next;
        next *= (seq_t)5;
        ++m;
    }

    /* Now p = 5^m, numer = K - p.  Compute the sum over j = 0..m-1 */
    seq_t numer = K - p;
    seq_t result = 0;
    seq_t pow5j = 1;      /* 5^j */
    seq_t pow10j = 1;     /* 10^j */

    for (idx_t j = 0; j < m; ++j) {
        seq_t denom = (seq_t)4 * pow5j;
        seq_t b = numer / denom;     /* floor division */
        seq_t digit = 1 + (b % 5);   /* (1 + b mod 5) */
        result += digit * pow10j;

        pow5j *= (seq_t)5;
        pow10j *= (seq_t)10;
    }

    return result;
}
