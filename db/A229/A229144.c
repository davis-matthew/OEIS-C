/*
 * A229144  Partial sums of (Fibonacci numbers mod 3).
 * https://oeis.org/A229144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^n (F_k mod 3). A period of length 8 occurs for
 *            the Fibonacci numbers modulo 3: [1,1,2,0,2,2,1,0], whose
 *            cumulative sums over a cycle are: cum[0] = 0, cum[1] = 1, cum[2]
 *            = 2, cum[3] = 4, cum[4] = 4, cum[5] = 6, cum[6] = 8, cum[7] = 9,
 *            cum[8] = 9. Hence a(n) = (n/8)*9 + cum[n%8]
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Number of complete 8‑term periods in the first n Fibonacci
       numbers (starting from F_1).  */
    idx_t full = n / 8;
    idx_t rem  = n % 8;

    /* Pre‑computed sums of a full period (9) and the partial
       sums for the remainder. */
    static const seq_t period_sum = 9;
    static const seq_t cum[9] = {0, 1, 2, 4, 4, 6, 8, 9, 9};

    /* Compute the total sum.  Convert 'full' to seq_t to avoid
       mixed‑type arithmetic.  For 64‑bit seq_t the intermediate
       product fits for all n whose result fits in seq_t because
       period_sum is small.  For an arbitrary‑precision seq_t this
       cast constructs a big integer. */
    seq_t sum = (seq_t)full * period_sum;
    sum += cum[rem];

    return sum;
}
