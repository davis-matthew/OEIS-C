/*
 * A166614  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 25·a(n-1) +
 *            25·a(n-2) + … + 25·a(n-11) – 325·a(n-12) (n ≥ 13) * and * a(12)
 *            = 25·a(11) + … + 25·a(1) – 325·a(0) + 1. * a(0) … a(11) are
 *            given by the initial values below
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[12] = {
    (seq_t)1,                 /* a(0) */
    (seq_t)27,                /* a(1) */
    (seq_t)702,               /* a(2) */
    (seq_t)18252,             /* a(3) */
    (seq_t)474552,            /* a(4) */
    (seq_t)12338352,          /* a(5) */
    (seq_t)320797152,         /* a(6) */
    (seq_t)8340725952,        /* a(7) */
    (seq_t)216858874752,      /* a(8) */
    (seq_t)5638330743552,     /* a(9) */
    (seq_t)146596599332352,   /* a(10) */
    (seq_t)3811511582641152   /* a(11) */
};

#define BUF_SIZE 20000   /* must be ≥ 13 to accommodate recurrence */

/* Memoisation buffer */
static seq_t buf[BUF_SIZE];
static idx_t max_computed = 11;   /* highest index computed so far */

/* Initialise the buffer with the first 12 terms (once) */
static void init_buffer(void)
{
    if (buf[0] == 0) {   /* static objects start zero‑initialised */
        for (idx_t i = 0; i <= 11; ++i) {
            buf[i] = init_vals[i];
        }
    }
}

/* Compute all terms up to n (inclusive), storing them in buf. */
static void compute_up_to(idx_t n)
{
    if (n <= max_computed) return;

    init_buffer();

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = buf[i-1] * (seq_t)25;
        sum += buf[i-2] * (seq_t)25;
        sum += buf[i-3] * (seq_t)25;
        sum += buf[i-4] * (seq_t)25;
        sum += buf[i-5] * (seq_t)25;
        sum += buf[i-6] * (seq_t)25;
        sum += buf[i-7] * (seq_t)25;
        sum += buf[i-8] * (seq_t)25;
        sum += buf[i-9] * (seq_t)25;
        sum += buf[i-10] * (seq_t)25;
        sum += buf[i-11] * (seq_t)25;
        sum -= buf[i-12] * (seq_t)325;

        /* The numerator contributes 1 only to the term a(12). */
        if (i == 12) sum += (seq_t)1;

        buf[i] = sum;
    }
    max_computed = n;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n < 12) return init_vals[n];

    if (n >= (idx_t)BUF_SIZE) {
        /* For safety we compute only up to the buffer limit; the caller
         * should not request values beyond BUF_SIZE-1 with this implementation. */
        compute_up_to((idx_t)BUF_SIZE - 1);
        return buf[BUF_SIZE - 1];
    }

    compute_up_to(n);
    return buf[n];
}
