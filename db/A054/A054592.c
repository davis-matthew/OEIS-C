/*
 * A054592  Number of disconnected labeled graphs with n nodes.
 * https://oeis.org/A054592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n*(n-1)/2) - A001187(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        (Number of disconnected labeled graphs).   */
/* Implementation:   */
/*   * Pre‑compute total[n] = 2^(n choose 2)          */
/*   * Use the classical recurrence for connected graphs:    */
/*         c(n) = total(n) - Σ_{k=1}^{n-1} C(n-1,k-1) * c(k) * total(n-k) */
/*   * Then a(n) = total(n) - c(n) = Σ_{k=1}^{n-1} C(n-1,k-1) * c(k) * total(n-k) */
/*   * Memoised in static arrays so that calls to a() are O(n) in the total */
/*     number of distinct arguments evaluated.                         */

#include <stdlib.h>

/* The build supplies the following typedefs */
/* extern typedef unsigned long long idx_t;   */
/* extern typedef ... seq_t;                 */

static seq_t *Ctotal = NULL;   /* total[n] = 2^(n choose 2)                */
static seq_t *Cconn  = NULL;   /* Cconn[n] = number of connected graphs   */
static seq_t *Cdisc  = NULL;   /* Cdisc[n] = number of disconnected graphs */
static size_t Csize  = 0;      /* number of pre‑computed entries */

/* Ensure arrays are grown to include index n.  */
static void ensure_size(idx_t n)
{
    if (n < (idx_t)Csize) return;          /* already computed */

    size_t new_size = (size_t)(n + 1);
    Ctotal = (seq_t *)realloc(Ctotal, new_size * sizeof(seq_t));
    Cconn  = (seq_t *)realloc(Cconn,  new_size * sizeof(seq_t));
    Cdisc  = (seq_t *)realloc(Cdisc,  new_size * sizeof(seq_t));

    /* If allocation failed, the program may terminate now. */
    if (!Ctotal || !Cconn || !Cdisc) exit(1);

    for (size_t i = Csize; i <= new_size - 1; ++i)
    {
        if (i == 0)
        {
            Ctotal[0] = (seq_t)1;  /* 2^0 */
            Cconn[0]  = (seq_t)1;  /* convention for recurrence */
            Cdisc[0]  = (seq_t)0;
            continue;
        }

        /* total[i] = total[i-1] << (i-1)  (multiply by 2^(i-1)) */
        Ctotal[i] = Ctotal[i-1];
        Ctotal[i] <<= (size_t)(i-1);

        /* Recurrence for number of connected graphs */
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;          /* C(i-1,0) for k=1 */

        for (idx_t k = 1; k <= i-1; ++k)
        {
            seq_t term = binom;
            term *= Cconn[(size_t)k];
            term *= Ctotal[(size_t)(i - k)];
            sum += term;

            if (k != i-1)                  /* avoid useless division at last step */
            {
                binom *= (seq_t)(i - k);
                binom /= (seq_t)k;
            }
        }

        Cconn[i] = Ctotal[i] - sum;
        Cdisc[i] = sum;                   /* total - connected = disconnected */
    }

    Csize = new_size;
}

/* Function demanded by the test harness */
seq_t a(idx_t n)
{
    ensure_size(n);
    return Cdisc[(size_t)n];
}
