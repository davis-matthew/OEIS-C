/*
 * A130470  Antidiagonal sums of triangular array T: T(j,k) = k*(j-k)! for k <
 *          j, T(j,k) = 1 for k = j; 1 <= k <= j.
 * https://oeis.org/A130470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{n-1, i≡n-1 (mod 2)} ((n+1−i)/2)·i! + (n%2?1:0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=1, a(3)=3, a(4)=8, a(5)=29, a(6)=135, a(7)=775, …   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static seq_t *a_buf      = NULL;  /* a values, 1..max_n */
    static idx_t  max_n      = 0;     /* highest n precomputed */
    static seq_t cur_fact    = 1;     /* fact[max_n]  */
    static seq_t sum_fe     = 0;     /* Σ i! for even i ≤ max_n */
    static seq_t sum_ie     = 0;     /* Σ i·i! for even i ≤ max_n */
    static seq_t sum_fo     = 0;     /* Σ i! for odd  i ≤ max_n */
    static seq_t sum_io     = 0;     /* Σ i·i! for odd  i ≤ max_n */

    /* extend precomputation if necessary */
    if (n > max_n)
    {
        size_t new_size = (size_t)(n + 1);          /* indices 0..n */
        a_buf = (seq_t *)realloc(a_buf, new_size * sizeof(seq_t));
        if (!a_buf) return 0;                    /* allocation failure */

        /* compute a(i) for i=max_n+1 .. n */
        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            seq_t fact_i = cur_fact * (seq_t)i;   /* fact[i]   */
            /* compute a(i) using sums up to i-1 */
            int parity = (int)((i - 1) & 1);      /* 0: even, 1: odd */
            seq_t P   = (parity ? sum_fo : sum_fe);  /* Σ fact[i] of matching parity */
            seq_t Q   = (parity ? sum_io : sum_ie);  /* Σ i·fact[i] of matching parity */
            seq_t temp = ((seq_t)i + 1) * P - Q;    /* ((n+1)·P – Q) */
            seq_t sum  = temp / 2;                 /* exact division by 2 */
            seq_t add  = (i & 1) ? (seq_t)1 : (seq_t)0;  /* add 1 if n odd */
            a_buf[(size_t)i] = sum + add;

            /* update cumulative sums with fact[i] */
            if (i & 1) {                /* i odd */
                sum_fo += fact_i;
                sum_io += (seq_t)i * fact_i;
            } else {                    /* i even */
                sum_fe += fact_i;
                sum_ie += (seq_t)i * fact_i;
            }
            cur_fact = fact_i;          /* fact[i] becomes current_fact for next step */
        }
        max_n = n;
    }
    return a_buf[(size_t)n];
}
