/*
 * A159607  G.f. A(x) satisfies A(x) = 1 + x*d/dx log(1 + x*A(x)^2).
 * https://oeis.org/A159607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A159607.
   Recurrence:
     a(0) = 1
     conv2[t] = Σ_{i+j=t} a(i)*a(j)  (ordered pairs)
     a(n) = (n+1)*conv2[n-1] – Σ_{j=0}^{n-1} a(j)*conv2[n-1-j]
   The code keeps the sequence and the convolution in two dynamic
   arrays that are expanded on demand.  For an arbitrary‑precision
   seq_t the code uses normal arithmetic operators; the cast from
   void* returned by realloc is added so the program compiles under
   both C11 and C++17. */

#include <stdlib.h>

/* static storage for the sequence and the convolution of order two */
static seq_t *a_seq          = NULL; /* a[0..]                          */
static size_t a_cap          = 0;     /* allocated capacity for a_seq     */
static size_t a_len          = 0;     /* highest computed index + 1       */

static seq_t *conv2_seq      = NULL; /* conv2[t] = Σ_{i+j=t} a(i)*a(j)   */
static size_t conv2_cap      = 0;     /* allocated capacity for conv2_seq */

/* Ensure storage for a_seq up to index n (inclusive).  If needed,
   also grow conv2_seq up to 2*n.  New elements are initialised to 0. */
static void ensure_capacity(idx_t n)
{
    /* grow a_seq if necessary */
    if (a_len <= (size_t)n) {
        size_t newcap = a_cap ? a_cap : 1;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *newa = (seq_t*)realloc(a_seq, newcap * sizeof(seq_t));
        if (!newa) exit(EXIT_FAILURE);
        /* initialise new cells to zero */
        for (size_t i = a_cap; i < newcap; ++i) newa[i] = 0;
        a_seq = newa;
        a_cap = newcap;
    }

    /* grow conv2_seq up to index 2*n if necessary */
    size_t need = 2 * (size_t)n + 1;
    if (conv2_cap < need) {
        size_t newcap = conv2_cap ? conv2_cap : 1;
        while (newcap < need) newcap <<= 1;
        seq_t *newc = (seq_t*)realloc(conv2_seq, newcap * sizeof(seq_t));
        if (!newc) exit(EXIT_FAILURE);
        for (size_t i = conv2_cap; i < newcap; ++i) newc[i] = 0;
        conv2_seq = newc;
        conv2_cap = newcap;
    }
}

/* Initialise the tables with a(0)=1 and conv2[0]=1.  */
static void init_tables(void)
{
    if (a_seq) return;          /* already initialised */

    a_seq   = (seq_t*)malloc(sizeof(seq_t));
    a_cap   = 1;
    a_len   = 1;
    a_seq[0] = 1;

    conv2_seq = (seq_t*)malloc(sizeof(seq_t));
    conv2_cap = 1;
    conv2_seq[0] = 1;           /* conv2[0] = a(0)*a(0) = 1 */
}

/* Public function as required */
seq_t a(idx_t n)
{
    init_tables();
    ensure_capacity(n);

    /* Compute missing terms up to n */
    for (idx_t i = a_len; i <= n; ++i) {
        if (i == 0) continue;   /* a(0) already set */

        /* first sum: (i+1)*conv2[i-1] */
        seq_t first = (seq_t)(i + 1) * conv2_seq[i - 1];

        /* second sum: Σ_{j=0}^{i-1} a(j)*conv2[i-1-j] */
        seq_t second = 0;
        for (idx_t j = 0; j < i; ++j) {
            second += a_seq[j] * conv2_seq[i - 1 - j];
        }

        a_seq[i] = first - second;

        /* update conv2 with the new a(i) */
        for (idx_t j = 0; j <= i; ++j) {
            seq_t prod = a_seq[i] * a_seq[j];
            if (j == i) {
                conv2_seq[i + j] += prod;        /* i==j, one term */
            } else {
                conv2_seq[i + j] += prod + prod; /* ordered pairs */
            }
        }
    }

    a_len = n + 1;
    return a_seq[n];
}
