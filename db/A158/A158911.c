/*
 * A158911  Numbers of the form 2^i*5^j - 1.
 * https://oeis.org/A158911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (the nth number of the form 2^i * 5^j) - 1 This
 *            implements the ordered sequence of numbers 2^i·5^j (i,j ≥ 0) and
 *            subtracts 1 to match the OEIS definition of A158911. The
 *            algorithm uses the classic 2‑prime Hamming–number generator,
 *            which runs in O(n) time and uses only integer arithmetic
 * Range:     u8 n <= 21, u16 n <= 68, u32 n <= 245, u64 n <= 929,
 *            u128 n <= 3621, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function has external linkage */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1; 0 is out of range */
        return (seq_t)0;

    const size_t N = (size_t)n;          /* number of terms to generate */

    /* Allocate an array to hold the first N numbers of the form 2^i*5^j */
    seq_t *seq = (seq_t *)malloc(N * sizeof(seq_t));
    if (!seq)
        abort();  /* allocation failure */

    /* Seed the sequence */
    seq[0] = (seq_t)1;         /* 2^0 * 5^0 */
    size_t i2 = 0, i5 = 0;     /* pointers for multiples of 2 and 5 */

    /* Generate the sequence in increasing order */
    for (size_t k = 1; k < N; ++k)
    {
        seq_t val2 = seq[i2] * (seq_t)2;
        seq_t val5 = seq[i5] * (seq_t)5;
        seq_t next = (val2 < val5) ? val2 : val5;
        seq[k] = next;

        /* Advance pointers that contributed to the new minimum */
        if (next == val2)
            ++i2;
        if (next == val5)
            ++i5;
    }

    /* The required term is the (n‑1)th element minus 1 */
    seq_t result = seq[N - 1] - (seq_t)1;

    free(seq);
    return result;
}
