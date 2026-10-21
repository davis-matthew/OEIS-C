/*
 * A066897  Total number of odd parts in all partitions of n.
 * https://oeis.org/A066897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} b(k) * p(n-k) Alternative recurrence derived
 *            from the generating functions: Let B_n = b(n) = number of odd
 *            divisors of n. Let the generalized pentagonal numbers be g_k =
 *            k(3k-1)/2 and k(3k+1)/2 with sign s_k = (-1)^{k+1} (= +1 for k
 *            odd, -1 for k even). Then a(0) = 0 a(n) = B_...
 * Range:     u8 n <= 11, u16 n <= 32, u32 n <= 104, u64 n <= 362,
 *            u128 n <= 1333, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

/*Implementation*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* term for offset 0 is unused */

    static idx_t max_n = 0;                       /* largest n computed so far */
    static seq_t *a_arr = NULL;                   /* array of a values   */
    static seq_t *b_arr = NULL;                   /* array of b(v)       */
    static int  *pent_val = NULL;                 /* generalized pentagon values   */
    static int  *pent_sign = NULL;                /* their signs (+1/-1)         */
    static size_t pent_len = 0;                   /* number of pentagonal terms   */

    /* Re‑compute everything if a larger n is requested. */
    if (n > max_n)
    {
        /* Allocate new arrays.  Size = n+1 to store indices 0..n. */
        size_t N = (size_t)n + 1;

        seq_t *new_a = (seq_t *)calloc(N, sizeof(seq_t));
        seq_t *new_b = (seq_t *)calloc(N, sizeof(seq_t));

        if (!new_a || !new_b) {          /* allocation failure */
            free(new_a); free(new_b);
            return (seq_t)0;             /* safe fallback */
        }

        /* -------- Compute b(k) = number of odd divisors of k -------- */
        new_b[0] = (seq_t)0;
        for (idx_t i = 1; i <= n; i += 2)           /* odd i only */
        {
            for (idx_t j = i; j <= n; j += i)
                new_b[j] += (seq_t)1;
        }

        /* -------- Build list of generalized pentagonal numbers -------- */
        /* The largest pentagonal number needed is <= n. */
        size_t estimated = 0;
        for (idx_t k = 1; ; ++k)
        {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > n && g2 > n) break;
            estimated += 2;
        }

        int *new_pv = (int *)malloc(estimated * sizeof(int));
        int *new_ps = (int *)malloc(estimated * sizeof(int));
        if (!new_pv || !new_ps) {           /* allocation failure */
            free(new_a); free(new_b);
            free(new_pv); free(new_ps);
            return (seq_t)0;
        }

        size_t idx = 0;
        for (idx_t k = 1; ; ++k)
        {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            int sign = (k & 1) ? +1 : -1;   /* (-1)^{k+1} */

            if (g1 <= n) { new_pv[idx] = (int)g1; new_ps[idx] = sign; ++idx; }
            if (g2 <= n) { new_pv[idx] = (int)g2; new_ps[idx] = sign; ++idx; }
            if (g1 > n && g2 > n) break;
        }

        /* Sort pentagonal numbers ascending (they are already roughly sorted,
           but sorting ensures correct ordering for the recurrence).  */
        for (size_t i = 0; i < idx - 1; ++i)
            for (size_t j = i + 1; j < idx; ++j)
                if (new_pv[j] < new_pv[i])
                {
                    int tmp = new_pv[i]; new_pv[i] = new_pv[j]; new_pv[j] = tmp;
                    tmp = new_ps[i]; new_ps[i] = new_ps[j]; new_ps[j] = tmp;
                }

        /* -------- Compute a(n) using the recurrence -------- */
        new_a[0] = (seq_t)0;                 /* base case */
        for (idx_t cur = 1; cur <= n; ++cur)
        {
            seq_t sum = new_b[cur];           /* B_cur */
            for (size_t i = 0; i < idx; ++i)
            {
                int val = new_pv[i];
                if (val > cur) break;
                if (new_ps[i] > 0)
                    sum += new_a[cur - val];
                else
                    sum -= new_a[cur - val];
            }
            new_a[cur] = sum;
        }

        /* Free previous static arrays (may be NULL on first run). */
        free(a_arr); free(b_arr);
        free(pent_val); free(pent_sign);

        /* Store new arrays and meta‑data. */
        a_arr = new_a;
        b_arr = new_b;
        pent_val = new_pv;
        pent_sign = new_ps;
        pent_len = idx;
        max_n = n;
    }

    return a_arr[n];
}
