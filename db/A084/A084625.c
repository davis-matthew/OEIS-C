/*
 * A084625  Binomial transform of A084624.
 * https://oeis.org/A084625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + Σ_{k=0..n} binomial(n,k) * floor(k*(k^2 + 12*k +
 *            47)/60)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 25, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers required for dynamic allocation */
#include <stddef.h>
#include <stdlib.h>

/* Helper that grows a pointer to an array of seq_t to at least “need”
   elements. The new capacity is doubled each time its size is insufficient. */
static void ensure_cap(void **p, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap << 1) : 1;
        while (new_cap < need) new_cap <<= 1;
        *p = realloc(*p, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* The required sequence */
seq_t a(idx_t n)
{
    /* Memoisation tables */
    static seq_t *seq      = NULL; /* a[0..] */
    static size_t seq_len  = 0, seq_cap = 0;

    static seq_t *binom    = NULL; /* current row of Pascal’s triangle */
    static size_t binom_cap = 0;

    static seq_t pow2 = 1;          /* holds 2^current_row */

    /* Expand the tables if the requested index is beyond what we have
       already computed.  All re‑allocations keep the previous contents. */
    if (n >= seq_len) {
        ensure_cap((void**)&seq,   &seq_cap,   n + 1);
        ensure_cap((void**)&binom, &binom_cap, n + 1);

        /* Initialise the very first row if we have not yet. */
        if (seq_len == 0) {
            binom[0] = 1;          /* C(0,0) */
            seq[0]   = 1;          /* a(0) = 1 */
            seq_len  = 1;
            pow2     = 1;          /* 2^0 */
        }

        /* Build rows up to n, using the current binomial coefficients. */
        for (size_t cur = seq_len; cur <= n; ++cur) {
            /* Create the new row of Pascal’s triangle.  The highest
               coefficient C(cur,cur) is 1.  Then we update the remaining
               coefficients in descending order. */
            binom[cur] = 1;
            for (size_t k = cur; k-- > 1; )
                binom[k] = binom[k] + binom[k - 1];

            /* Update pow2 to 2^cur.  cur is at least 1 here. */
            pow2 = pow2 * 2;

            /* Compute the requested value: 2^cur + Σ binom * floor(...). */
            seq_t sum = pow2;
            for (size_t k = 0; k <= cur; ++k) {
                seq_t ki  = (seq_t)k;
                seq_t ksq = ki * ki;
                seq_t num = ki * (ksq + 12 * ki + 47);
                seq_t bk  = num / 60;            /* floor division */
                seq_t prod = binom[k] * bk;
                sum = sum + prod;
            }
            seq[cur] = sum;
            ++seq_len;
        }
    }

    return seq[n];
}
