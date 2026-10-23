/*
 * A048859  A sieve: keep the first 2 numbers, delete the next 3 numbers; keep
 *          the next 3 numbers, delete the next 4 numbers; keep the next 4
 *          numbers, delete the next 5 numbers; and so on. In other words,
 *          keep the next k numbers and delete the next k+1 numbers, for k =
 *          2, 3, ...
 * https://oeis.org/A048859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by simulating the keep‑then‑delete sieve: for k =
 *            2,3,… keep k numbers then delete k+1 numbers
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* sequence defined for n >= 1 */

    idx_t k = 2;
    idx_t cum_keep = 2;           /* sum_{i=2}^{k} i */

    while (cum_keep < n)
    {
        ++k;
        cum_keep += k;            /* add the next keep block size */
    }

    idx_t prev_total = cum_keep - k;      /* total kept before this block */
    idx_t offset = n - prev_total - 1;    /* position inside the block (0‑based) */

    seq_t start_k = (seq_t)k * (seq_t)k - 3; /* first kept number of block k */
    return start_k + (seq_t)offset;          /* nth kept number */
}
