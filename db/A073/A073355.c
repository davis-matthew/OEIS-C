/*
 * A073355  Sum of squarefree kernels of numbers <= n.
 * https://oeis.org/A073355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} rad(k) (rad(k) = square‑free kernel of k)
 * Range:     u8 n <= 25, u16 n <= 430, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes spf up to n and builds rad(k) using the
   recurrence: rad(1)=1; for k>1 let p=spf(k), m=k/p;
   if (m % p == 0) rad(k)=rad(m); else rad(k)=rad(m)*p.
   It then accumulates a prefix sum to return a(n).              */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* allocate auxiliary arrays of size n+1                 */
    idx_t size = n + 1;
    idx_t *spf    = (idx_t *)calloc(size, sizeof(idx_t));
    seq_t *rad    = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *prefix = (seq_t *)calloc(size, sizeof(seq_t));

    if (!spf || !rad || !prefix) {
        /* on allocation failure terminate the program – the
           test harness treats this as a failure.            */
        abort();
    }

    /* linear unused: compute smallest prime factor for each number */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if ((i * i) <= n) {     /* fill multiples */
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    /* rad(1)=1 and prefix sum starts here */
    rad[1] = 1;
    prefix[1] = 1;

    /* build rad(k) for k>=2 and accumulate the prefix sum */
    for (idx_t k = 2; k <= n; ++k) {
        idx_t p = spf[k];
        idx_t m = k / p;
        if (m % p == 0)            /* p divides m */
            rad[k] = rad[m];
        else
            rad[k] = rad[m] * (seq_t)p;
        prefix[k] = prefix[k - 1] + rad[k];
    }

    seq_t result = prefix[n];

    free(spf);
    free(rad);
    free(prefix);

    return result;
}
