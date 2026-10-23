/*
 * A122056  Expansion of g.f. x^2/((1 - x)^4*(1 + x)*(1 + x^2)*(1 + x^4)).
 * https://oeis.org/A122056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 143, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A122056
   Recurrence (for n > 10):
     a(n) = 3*a(n‑1) – 3*a(n‑2) + a(n‑3) + a(n‑8) – 3*a(n‑9) + 3*a(n‑10) – a(n‑11)
   Initial values:
     a(0)=0,a(1)=0,a(2)=1,a(3)=3,a(4)=6,a(5)=10,a(6)=15,a(7)=21,
     a(8)=28,a(9)=36,a(10)=46,a(11)=58
   This implementation keeps a memoised table that grows on demand,
   suitable for both built‑in unsigned integer types and arbitrary‑precision
   integer classes compiled as C++17. */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free, realloc (C) */

#ifdef __cplusplus
/* In C++, use operator new/delete for non‑trivial seq_t types. */
static seq_t *allocate_seq_array(size_t n)
{
    return new seq_t[n];
}
static void free_seq_array(seq_t *p)
{
    delete[] p;
}
#else
/* In C, use malloc/free. */
static seq_t *allocate_seq_array(size_t n)
{
    return (seq_t *)malloc(n * sizeof(seq_t));
}
static void free_seq_array(seq_t *p)
{
    free(p);
}
#endif

/* ---- memoisation table ------------------------------------------- */
static seq_t *dp = NULL;          /* array of stored values */
static idx_t dp_max = 0;          /* highest index that has been filled */
static size_t dp_cap = 0;         /* allocated capacity of dp */

/* ---- initial values (a(0) … a(11)) -------------------------------- */
static const seq_t initial[12] = {
    (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)10,
    (seq_t)15, (seq_t)21, (seq_t)28, (seq_t)36, (seq_t)46, (seq_t)58
};

/* ---- ensure dp is initialised ------------------------------------ */
static void dp_init(void)
{
    if (dp != NULL) return;               /* already initialised */
    dp_cap = 12;
    dp = allocate_seq_array(dp_cap);
    for (idx_t i = 0; i < 12; ++i)
        dp[i] = initial[i];
    dp_max = 11;
}

/* ---- grow the table so that it can hold index >= n ---------------- */
static void dp_ensure_size(idx_t n)
{
    if (dp == NULL) dp_init();
    if ((idx_t)dp_cap > n) return;

    size_t new_cap = dp_cap;
    while ((idx_t)new_cap <= n) new_cap <<= 1;   /* double until large enough */

    seq_t *new_dp = allocate_seq_array(new_cap);
    /* copy existing values */
    for (idx_t i = 0; i <= dp_max; ++i)
        new_dp[i] = dp[i];

    /* free old array */
    free_seq_array(dp);

    dp = new_dp;
    dp_cap = new_cap;
}

/* ---- fill the table up to n ------------------------------------- */
static void dp_compute_up_to(idx_t n)
{
    if (n <= dp_max) return;

    dp_ensure_size(n);

    for (idx_t k = dp_max + 1; k <= n; ++k) {
        /* recurrence: a(k) = 3*a(k-1) – 3*a(k-2) + a(k-3) + a(k-8)
                         – 3*a(k-9) + 3*a(k-10) – a(k-11) */
        seq_t term1 = (seq_t)3 * dp[k - 1];
        seq_t term2 = (seq_t)3 * dp[k - 2];
        seq_t term3 = dp[k - 3];
        seq_t term4 = dp[k - 8];
        seq_t term5 = (seq_t)3 * dp[k - 9];
        seq_t term6 = (seq_t)3 * dp[k - 10];
        seq_t term7 = dp[k - 11];

        seq_t val = term1;
        val -= term2;
        val += term3;
        val += term4;
        val -= term5;
        val += term6;
        val -= term7;

        dp[k] = val;
    }
    dp_max = n;
}

/* ---- public function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 11)
        return initial[n];
    dp_compute_up_to(n);
    return dp[n];
}
