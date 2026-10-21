/*
 * A057514  Number of peaks in mountain ranges encoded by A014486, number of
 *          leaves in the corresponding rooted plane trees (the root node is
 *          never counted as a leaf).
 * https://oeis.org/A057514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005811(A014486(n))/2
 * Range:     u8 n <= 3485, u16 n <= 3485, u32 n <= 3485, u64 n <= 3485,
 *            u128 n <= 3485, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function interprets n as the rank of a Dyck word in the natural
   concatenation of all Dyck words of increasing semilength.
   It then decodes the word of the appropriate length and counts its
   peaks.  Peaks are occurrences of the pattern UD in the word.  The
   algorithm runs in O(m^2) time with m the semilength of the word;
   for n up to 10000 m ≤ 10 so the routine is very fast. */

#include <stdbool.h>   /* for bool */

/* Maximum semilength needed for n up to 10⁴.  15 is more than enough
   for portability and keeps the tables small. */
#define MAX_M 15
#define MAX_STEPS (2 * MAX_M)

/* Pre‑computed tables, initialized on first call. */
static idx_t catalan[MAX_M + 1];
static idx_t cum_catalan[MAX_M + 1];
static idx_t dp[MAX_STEPS + 1][MAX_M + 1];
static bool   tables_inited = false;

/* ----------------------------------------------------------------------- */
/* Initialise the Catalan numbers and the completion DP table.               */
/* ----------------------------------------------------------------------- */
static void init_tables(void)
{
    if (tables_inited)
        return;
    tables_inited = true;

    /* Catalan numbers: C0 = 1, C_{k+1} = C_k * (4k+2)/(k+2) */
    catalan[0] = 1;
    for (idx_t k = 0; k < MAX_M; ++k) {
        catalan[k + 1] = catalan[k] * (4 * k + 2) / (k + 2);
    }

    /* Cumulative sums of Catalan numbers.  cum_catalan[m] = C0 + … + Cm */
    cum_catalan[0] = catalan[0];
    for (idx_t m = 1; m <= MAX_M; ++m)
        cum_catalan[m] = cum_catalan[m - 1] + catalan[m];

    /* dp[rem][bal] = number of Dyck suffixes of length rem
       that can be appended to a prefix having current balance bal. */
    for (idx_t bal = 0; bal <= MAX_M; ++bal)
        dp[0][bal] = (bal == 0) ? 1 : 0;
    for (idx_t rem = 1; rem <= MAX_STEPS; ++rem) {
        for (idx_t bal = 0; bal <= MAX_M; ++bal) {
            idx_t val = 0;
            if (bal > 0)          /* place a down step */
                val += dp[rem - 1][bal - 1];
            if (bal + 1 <= MAX_M) /* place an up step */
                val += dp[rem - 1][bal + 1];
            dp[rem][bal] = val;
        }
    }
}

/* ----------------------------------------------------------------------- */
/* Public entry point required by the test harness.                          */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_tables();               /* Ensure tables are ready for use. */

    /* Special case: the empty word (n == 0) has no peaks. */
    if (n == 0)
        return (seq_t)0;

    /* Determine the semilength m of the Dyck word corresponding to n. */
    idx_t m = 0;
    while (m < MAX_M && n >= cum_catalan[m])
        ++m;

    /* Rank of the word within the class of semilength m. */
    idx_t r = n - (m == 0 ? 0 : cum_catalan[m - 1]);

    const idx_t len = 2 * m;     /* total number of steps in the word. */
    idx_t       bal = 0;         /* current balance (ups – downs).   */
    int         last = -1;       /* -1 = none, 0 = down, 1 = up.    */
    seq_t       peaks = 0;       /* result counter.                  */

    for (idx_t pos = 0; pos < len; ++pos) {
        idx_t rem = len - pos;   /* steps remaining, including current. */
        idx_t count_down = 0;    /* completions if we put a down here.  */
        if (bal > 0)
            count_down = dp[rem - 1][bal - 1];

        int step;                /* 0 = down, 1 = up. */
        if (r < count_down) {    /* choose a down step.    */
            step = 0;
            --bal;
        } else {                 /* choose an up step.     */
            r -= count_down;
            step = 1;
            ++bal;
        }

        /* A peak occurs when we see an up step followed immediately by
           a down step (pattern UD). */
        if (last == 1 && step == 0)
            ++peaks;

        last = step;
    }

    return peaks;
}
