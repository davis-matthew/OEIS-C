/*
 * A144961  Eigentriangle whose left border is the Padovan sequence, and whose
 *          right border and row sums are a modified Fibonacci sequence.
 * https://oeis.org/A144961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = Fib( max(k-1,1) ) * Padovan(n-k), * where n = r*(r+1)/2 +
 *            k, 0 ≤ k ≤ r
 * Range:     u8 n <= 134, u16 n <= 376, u32 n <= 1273, u64 n <= 4654,
 *            u128 n <= 9635, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the row r that contains n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start   = r * (r + 1) / 2;   /* first index of row r */
    idx_t k       = n - start;        /* column within the row */
    idx_t pad_idx = r - k;             /* index for Padovan sequence */
    idx_t fib_idx = (k <= 1) ? 1 : (k - 1);  /* index for Fibonacci sequence */

    /* Compute Padovan(pad_idx):
       Pad[0] = 1, Pad[1] = 0, Pad[2] = 0, Pad[i] = Pad[i-2] + Pad[i-3] (i ≥ 3) */
    seq_t pad_val;
    if (pad_idx == 0) {
        pad_val = 1;
    } else if (pad_idx == 1 || pad_idx == 2) {
        pad_val = 0;
    } else {
        seq_t a0 = 1;   /* Pad[i-3]  */
        seq_t a1 = 0;   /* Pad[i-2]  */
        seq_t a2 = 0;   /* Pad[i-1]  */
        seq_t cur;
        for (idx_t i = 3; i <= pad_idx; ++i) {
            cur = a0 + a1;   /* Pad[i] = Pad[i-3] + Pad[i-2] */
            a0 = a1;
            a1 = a2;
            a2 = cur;
        }
        pad_val = a2;
    }

    /* Compute Fib(fib_idx):
       Fib[0] = 0, Fib[1] = 1, Fib[i] = Fib[i-1] + Fib[i-2] (i ≥ 2) */
    seq_t fib_val;
    if (fib_idx == 0) {
        fib_val = 0;
    } else if (fib_idx == 1) {
        fib_val = 1;
    } else {
        seq_t f0 = 0;
        seq_t f1 = 1;
        seq_t f2;
        for (idx_t i = 2; i <= fib_idx; ++i) {
            f2 = f0 + f1;
            f0 = f1;
            f1 = f2;
        }
        fib_val = f1;
    }

    return pad_val * fib_val;
}
