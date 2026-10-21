/*
 * A076935  a(1)=1,
 *          a(n)=n-a(n-1)+a(a(n-1))-a(a(a(n-1)))+a(a(a(a(n-1))))-a(a(a(a(a(n-1))))).
 * https://oeis.org/A076935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, * a(n) = n - a(n-1) + a(a(n-1)) - a(a(a(n-1))) + *
 *            a(a(a(a(n-1)))) - a(a(a(a(a(n-1))))) * (n ≥ 2)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/*  Maximum n we expect to handle – sufficient for 10 000. */
#define MAX_SIZE 10001

static seq_t a_vals[MAX_SIZE];
static idx_t a_last = 0;

/* Compute a(n) recursively, memoising all values up to n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety – not used */

    /* Initialise base element once. */
    if (a_last == 0) {
        a_vals[0] = 0;              /* dummy, never read */
        a_vals[1] = 1;              /* a(1)=1 */
        a_last = 1;
    }

    /* Calculate missing entries up to n. */
    while (a_last < n) {
        ++a_last;

        seq_t prev   = a_vals[a_last - 1];
        seq_t s      = (seq_t)a_last;       /* start with n */
        s -= prev;                         /* - a(n‑1)      */

        seq_t t1 = a_vals[(idx_t)prev];     /* a(a(n‑1))     */
        s += t1;

        seq_t t2 = a_vals[(idx_t)t1];       /* a(a(a(n‑1)))  */
        s -= t2;

        seq_t t3 = a_vals[(idx_t)t2];       /* a(a(a(a(n‑1)))) */
        s += t3;

        seq_t t4 = a_vals[(idx_t)t3];       /* a(a(a(a(a(n‑1))))) */
        s -= t4;

        a_vals[a_last] = s;
    }

    return a_vals[n];
}
