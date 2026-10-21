/*
 * A088208  Table read by rows where T(0,0)=1; n-th row has 2^n terms
 *          T(n,j),j=0 to 2^n-1. For j==0 mod 2, T(n+1,2j)=T(n,j) and
 *          T(n+1,2j+1)=T(n,j)+2^n. For j==1 mod 2, T(n+1,2j+1)=T(n,j) and
 *          T(n+1,2j)=T(n,j)+2^n.
 * https://oeis.org/A088208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   * This function implements the triangle T(n,j) defined in
 *            A088208: * * T(0,0)=1 * For j even: T(n+1,2j)=T(n,j) ,
 *            T(n+1,2j+1)=T(n,j)+2^n * For j odd : T(n+1,2j+1)=T(n,j) ,
 *            T(n+1,2j)=T(n,j)+2^n * * The sequence a(n) is the row‑major
 *            flattening of this triangle: * a(n) = T(r, j) where r =
 *            ⌊log2(n+1)...
 * Range:     u8 n <= 424, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r and column j for the given flattened index n */
    idx_t nplus = n + 1;                     /* n+1 > 0 for n >= 0            */
    idx_t r;
    if (nplus == 0)                          /* only possible if n == UINT64_MAX */
        r = 63;                              /* highest possible row index       */
    else
        r = 63 - __builtin_clzll(nplus);     /* floor(log2(n+1))                */

    /* Row r contains the indices from (2^r-1) to (2^{r+1}-2) */
    idx_t offset = ((idx_t)1 << r) - 1;      /* 2^r - 1                         */
    idx_t j = n - offset;                    /* column within the row           */

    /* Compute T(r,j) with the iterative parity‑based method */
    seq_t val = (seq_t)1;
    seq_t parent_parity = 0;

    for (idx_t i = r; i-- > 0;) {            /* bits from r-1 downto 0          */
        seq_t child_parity = (seq_t)((j >> i) & 1);
        if (parent_parity != child_parity) {
            val += (seq_t)1 << (r - 1 - i);  /* add 2^{(# bits remaining)}      */
        }
        parent_parity = child_parity;
    }

    return val;
}
