/*
 * A013215  Expansion of e.g.f. sinh(log(x+1) - sin(x)).
 * https://oeis.org/A013215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n/n! in sinh(log(1+x) - sin(x))
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Let g_k = k! * coefficient of x^k in log(1+x)-sin(x),
   g_k = (-1)^{k+1}*(k-1)! - [k odd ? (-1)^{(k-1)/2} : 0].
   Let e_plus[n] = coefficient of x^n/n! in exp(G(x)) where G(x)=∑ g_k x^k/k!.
   Then e_plus[n] satisfies: e_plus[0] = 1,
     e_plus[n] = ∑_{k=1}^n binom(n-1,k-1)*g_k*e_plus[n-k].
   Similarly e_minus[n] (for exp(-G)) satisfies
     e_minus[0] = 1, e_minus[n] = -∑_{k=1}^n binom(n-1,k-1)*g_k*e_minus[n-k].
   The desired sequence is a[n] = (e_plus[n]-e_minus[n])/2. */
/* All operations are integer; seq_t is an arbitrary‑precision integer type. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *fact_arr = NULL;      /* factorials: fact[i] = i!  */
static seq_t *g_arr   = NULL;       /* g[i] as defined above   */
static seq_t *e_plus_arr = NULL;    /* e^G coefficients        */
static seq_t *e_minus_arr = NULL;   /* e^-G coefficients       */
static seq_t *a_arr      = NULL;    /* final sequence a[n]     */
static idx_t current_max = 0;       /* highest n computed       */
static int initialized = 0;         /* flag for first init     */

static void ensure_up_to(idx_t n)
{
    if (!initialized) {
        /* initial allocation for index 0 */
        fact_arr     = (seq_t*)malloc(sizeof(seq_t));
        g_arr        = (seq_t*)malloc(sizeof(seq_t));
        e_plus_arr   = (seq_t*)malloc(sizeof(seq_t));
        e_minus_arr  = (seq_t*)malloc(sizeof(seq_t));
        a_arr        = (seq_t*)malloc(sizeof(seq_t));

        fact_arr[0]   = 1;
        g_arr[0]      = 0;          /* unused */
        e_plus_arr[0] = 1;
        e_minus_arr[0] = 1;
        a_arr[0]       = 0;         /* a(0) = 0 */
        current_max    = 0;
        initialized = 1;
    }
    if (n <= current_max) return;

    size_t new_size = (size_t)n + 1;
    fact_arr     = (seq_t*)realloc(fact_arr, new_size * sizeof(seq_t));
    g_arr        = (seq_t*)realloc(g_arr, new_size * sizeof(seq_t));
    e_plus_arr   = (seq_t*)realloc(e_plus_arr, new_size * sizeof(seq_t));
    e_minus_arr  = (seq_t*)realloc(e_minus_arr, new_size * sizeof(seq_t));
    a_arr        = (seq_t*)realloc(a_arr, new_size * sizeof(seq_t));

    /* compute from current_max+1 up to n */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        /* factorial */
        fact_arr[i] = fact_arr[i-1] * (seq_t)i;

        /* coefficient g[i] */
        seq_t sign1 = ((i & 1) ? 1 : -1);                        /* (-1)^(i+1) */
        seq_t temp  = sign1 * fact_arr[i-1];                      /* (i-1)! * sign1 */
        if (i & 1) { /* i is odd */
            idx_t half = (i - 1) / 2;                            /* (i-1)/2 */
            seq_t sign2 = ((half & 1) ? -1 : 1);                 /* (-1)^half */
            g_arr[i] = temp - sign2;
        } else {
            g_arr[i] = temp;
        }

        /* recurrence for e_plus and e_minus */
        seq_t bin    = 1;          /* binom(i-1,0) */
        seq_t sum_pl = 0;
        seq_t sum_mi = 0;

        for (idx_t k = 1; k <= i; ++k) {
            seq_t prod = bin * g_arr[k];
            /* e_plus part */
            sum_pl += prod * e_plus_arr[i - k];
            /* e_minus part (note the minus sign) */
            sum_mi -= prod * e_minus_arr[i - k];

            /* update binom for next k, unless last */
            if (k < i) {
                seq_t num = i - k;
                bin = bin * num / k;
            }
        }

        e_plus_arr[i]  = sum_pl;
        e_minus_arr[i] = sum_mi;

        /* final a[n] */
        a_arr[i] = (e_plus_arr[i] - e_minus_arr[i]) / 2;
    }

    current_max = n;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_arr[n];
}
