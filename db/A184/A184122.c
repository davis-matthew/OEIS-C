/*
 * A184122  Number of nX2 0..3 arrays with rows and columns in nondecreasing
 *          order.
 * https://oeis.org/A184122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×2 arrays with entries 0..3, rows and columns
 *            nondecreasing. Computed by the 16‑term recurrence a(n) = 16
 *            a(n-1) – 120 a(n-2) + 560 a(n-3) – 1820 a(n-4) + 4368 a(n-5) –
 *            8008 a(n-6) + 11440 a(n-7) – 12870 a(n-8) + 11440 a(n-9) – 8008
 *            a(n-10) + 4368 a(n-11) – 1820 a(n-12) + 560 a(...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 20, u64 n <= 115, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#  include <new>
#endif

/* Binomial coefficients C(16,k) for k = 0..16 */
static const unsigned int BINOM[17] = {
    1,     16,    120,   560,   1820,
    4368,  8008,  11440, 12870, 11440,
    8008,  4368,  1820,   560,   120,
    16,     1
};

/* Array of computed values, index starting at 1; index 0 unused.
   Values stored as seq_t (unsigned integer or big‑int). */
static seq_t *A_vals = NULL;
static idx_t   A_cap  = 0;   /* size of allocated array (elements 0..A_cap-1) */
static idx_t   A_max  = 0;   /* largest index that has been computed */

/* Ensure that A_vals has capacity at least n+1 elements (indices 0..n). */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= A_cap) return;

    idx_t new_cap = n + 1;
    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap];
#else
    new_arr = (seq_t*)malloc(sizeof(seq_t) * (size_t)new_cap);
#endif

    /* Copy existing values */
    for (idx_t i = 0; i < A_cap; ++i) new_arr[i] = A_vals[i];
    /* Initialise new slots to zero */
    for (idx_t i = A_cap; i < new_cap; ++i) new_arr[i] = 0;

#ifdef __cplusplus
    delete[] A_vals;
#else
    free(A_vals);
#endif
    A_vals = new_arr;
    A_cap  = new_cap;
}

/* Initialise the table with the first 16 terms.  Called on first use. */
static void init_table(void)
{
    if (A_vals) return;          /* already initialised */

    const idx_t init_cap = 17;    /* indices 0..16 */
#ifdef __cplusplus
    A_vals = new seq_t[init_cap];
#else
    A_vals = (seq_t*)malloc(sizeof(seq_t) * (size_t)init_cap);
#endif
    for (idx_t i = 0; i < init_cap; ++i) A_vals[i] = 0;

    /* a(1) .. a(16) */
    A_vals[1]  = 10;
    A_vals[2]  = 86;
    A_vals[3]  = 561;
    A_vals[4]  = 2861;
    A_vals[5]  = 12068;
    A_vals[6]  = 43947;
    A_vals[7]  = 142376;
    A_vals[8]  = 419213;
    A_vals[9]  = 1139569;
    A_vals[10] = 2894178;
    A_vals[11] = 6931366;
    A_vals[12] = 15770131;
    A_vals[13] = 34291532;
    A_vals[14] = 71619470;
    A_vals[15] = 144269412;
    A_vals[16] = 281284873;

    A_cap = init_cap;
    A_max = 16;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 */

    init_table();

    /* Return already computed value */
    if (n <= A_max) return A_vals[n];

    /* Ensure array large enough */
    ensure_capacity(n);

    /* Compute terms up to n using the recurrence */
    for (idx_t i = A_max + 1; i <= n; ++i)
    {
        /* Separate positive and negative contributions to avoid
           negative intermediate values. */
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int k = 1; k <= 16; ++k)
        {
            seq_t term = A_vals[i - k] * (seq_t)BINOM[k];
            if (k & 1)          /* k odd → positive coefficient */
                pos_sum += term;
            else                /* k even → negative coefficient */
                neg_sum += term;
        }
        A_vals[i] = pos_sum - neg_sum;    /* pos_sum >= neg_sum */
    }

    A_max = n;
    return A_vals[n];
}
