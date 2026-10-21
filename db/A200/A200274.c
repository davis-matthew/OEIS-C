/*
 * A200274  Number of 0..n arrays x(0..6) of 7 elements with zero 4th
 *          differences.
 * https://oeis.org/A200274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A200274 is counted by the empirical recurrence a(n) =
 *            a(n-5)+a(n-9)+a(n-10)-a(n-14)-a(n-15)-a(n-19)
 *            +a(n-20)+a(n-24)-a(n-25)-a(n-29)-a(n-30)
 *            +a(n-34)+a(n-35)+a(n-36)+a(n-39)-a(n-41)-a(n-44)
 *            -a(n-45)-a(n-46)+a(n-50)+a(n-51)+a(n-55)
 *            -a(n-56)-a(n-60)+a(n-61)+a(n-65)+a(n-66)
 *            -a(n-70)-a(n-71)-a(n-75)+...
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  seq_t and idx_t are provided by the build system.  */
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Cached values a(1)..a(max).  The array is indexed by the
    ordinary sequence index, so a[1] holds a(1).  */
static seq_t *A = NULL;     /* pointer to cache array             */
static idx_t  computed = 0; /* highest computed index (>= 80)    */
static size_t allocated = 0;/* current allocation size (index+1) */

/* ------------------------------------------------------------------ */
/*  Pre‑compute a(1)..a(80) by brute‑force enumeration of the 4
    determining parameters  (x0..x3).  The enumeration is fast
    because only (81)^4 ≈ 4.3·10^7 iterations are performed.  */
static void precompute_initial(void)
{
    const idx_t MAXN = 80;               /* maximum n to pre‑compute     */
    unsigned long long count[81] = {0};  /* count[max] for max = 0..80   */

    /* Enumerate all 4‑tuples (x0,x1,x2,x3) with components 0..MAXN.  */
    for (idx_t x0 = 0; x0 <= MAXN; ++x0)
    {
        for (idx_t x1 = 0; x1 <= MAXN; ++x1)
        {
            for (idx_t x2 = 0; x2 <= MAXN; ++x2)
            {
                for (idx_t x3 = 0; x3 <= MAXN; ++x3)
                {
                    /* Compute the remaining terms of the 7‑term
                       array.  All are integers.                  */
                    int x4 = -(int)x0 + 4 * (int)x1 - 6 * (int)x2 + 4 * (int)x3;
                    int x5 = -4 * (int)x0 + 15 * (int)x1 - 20 * (int)x2 + 10 * (int)x3;
                    int x6 = -10 * (int)x0 + 36 * (int)x1 - 45 * (int)x2 + 20 * (int)x3;

                    /* Discard if any term is outside 0..MAXN.      */
                    if (x4 < 0 || x5 < 0 || x6 < 0)                 continue;
                    if (x4 > (int)MAXN || x5 > (int)MAXN || x6 > (int)MAXN)
                        continue;

                    /* Determine the maximum element of the 7‑term array. */
                    int max_val = x0;
                    if (x1 > max_val) max_val = x1;
                    if (x2 > max_val) max_val = x2;
                    if (x3 > max_val) max_val = x3;
                    if (x4 > max_val) max_val = x4;
                    if (x5 > max_val) max_val = x5;
                    if (x6 > max_val) max_val = x6;

                    /* Count arrays whose maximum equals max_val. */
                    if (max_val <= (int)MAXN)
                        count[max_val] += 1ULL;
                }
            }
        }
    }

    /* Convert the per‑maximum counts into the cumulative sequence
       values: a(n) = Σ_{k=0}^n count[k].  */
    seq_t cum = 0;
    for (idx_t v = 0; v <= MAXN; ++v)
    {
        cum += (seq_t)count[v];
        if (v >= 1)               /* a(1) corresponds to v = 1 */
            A[v] = cum;
    }
    computed = MAXN;            /* highest computed index now 80   */
}

/* ------------------------------------------------------------------ */
/*  Offsets and signs for the empirical recurrence.  The arrays must
    have the same length.                                          */
static const int offs[31] = {
    5,  9, 10, 14, 15, 19, 20, 24, 25, 29, 30, 34, 35, 36, 39,
   41, 44, 45, 46, 50, 51, 55, 56, 60, 61, 65, 66, 70, 71, 75, 80
};
static const int signs[31] = {
    +1, +1, +1, -1, -1, -1, +1, +1, -1, -1, -1, +1, +1, +1, +1,
    -1, -1, -1, -1, +1, +1, +1, -1, -1, +1, +1, +1, -1, -1, -1, +1
};

/* ------------------------------------------------------------------ */
/*  Main sequence function.  The function is thread‑safe only if the
    caller guarantees single‑threaded access, as is typical in the
    sequence generator framework.  */
seq_t a(idx_t n)
{
    /* Initialise cache once.  */
    if (A == NULL) {
        allocated = 81;                 /* indices 0 … 80  (offset 1) */
        A = (seq_t *)malloc(allocated * sizeof(seq_t));
        if (!A) return (seq_t)0;       /* memory error */
        /* Zero A[0] to avoid accidental use.       */
        A[0] = 0;
        precompute_initial();            /* fills A[1..80], sets computed */
    }

    /* Return cached value if available.  */
    if (n <= 80)
        return A[n];

    /* Resize cache if necessary.  */
    if ((size_t)n >= allocated) {
        size_t new_alloc = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(A, new_alloc * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* memory error */
        A = tmp;
        allocated = new_alloc;
    }

    /* Extend the sequence up to n using the empirical
       recurrence.  */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (int j = 0; j < 31; ++j) {
                idx_t idx = i - (idx_t)offs[j];
                if (signs[j] > 0)
                    sum += A[idx];
                else
                    sum -= A[idx];
            }
            A[i] = sum;
        }
        computed = n;
    }

    return A[n];
}
