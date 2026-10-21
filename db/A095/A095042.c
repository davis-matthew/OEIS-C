/*
 * A095042  One of two (v,k,lambda)=(31,15,7) cyclic difference sets. The
 *          other one is A095041.
 * https://oeis.org/A095042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) is the n‑th integer that is a quadratic residue modulo 31.
 *            a(n) = Q[(n-1) mod 15] + 31 * floor((n-1)/15), where Q is the
 *            sorted list of distinct residues of i² (mod 31)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t residues[15];
    static int initialized = 0;

    if (!initialized) {
        /* compute the 15 distinct quadratic residues modulo 31 */
        int seen[31] = {0};
        seq_t tmp[15];
        int cnt = 0;

        for (idx_t i = 1; i <= 30; ++i) {
            uint64_t r = (i * i) % 31;
            if (!seen[(int)r]) {
                seen[(int)r] = 1;
                tmp[cnt++]   = (seq_t)r;   /* cnt will become 15 */
            }
        }

        /* sort the residues */
        for (int i = 0; i < cnt; ++i) {
            for (int j = i + 1; j < cnt; ++j) {
                if (tmp[i] > tmp[j]) {
                    seq_t t   = tmp[i];
                    tmp[i]    = tmp[j];
                    tmp[j]    = t;
                }
            }
        }

        for (int i = 0; i < 15; ++i)
            residues[i] = tmp[i];

        initialized = 1;
    }

    if (n == 0)
        return 0;          /* unspecified for n < 1 */

    idx_t blocks = (n - 1) / 15;   /* number of complete 31‑blocks */
    idx_t pos    = (n - 1) % 15;   /* position within the block */

    seq_t val = (seq_t)blocks * 31 + residues[pos];
    return val;
}
