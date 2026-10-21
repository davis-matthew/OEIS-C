/*
 * A226898  Hooley's Delta function: maximum number of divisors of n in [u,
 *          e*u] for all u. (Here e is Euler's number 2.718... = A001113.)
 * https://oeis.org/A226898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, core
 * Formula:   a(n) = max_{d|n} (# { e ∈ D(n) | d ≤ e ≤ floor(e·d) })
 * Range:     u8 n <= 834, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorise_uint64, sort_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  D(n) = set of divisors of n, e = 2.71828… (Euler's number)               */
/* This routine works for any seq_t/idx_t pair that supports the operators*/
/* used below.  The implementation factors n by trial division (fast for n ≤1024*10^4) and*/
/* enumerates all its divisors.  A simple insertion sort orders them, after which*/
/* a sliding window counts how many lie in each interval [d, floor(e·d)].      */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factorise_uint64.h"
#include "../utils/sort_divs.h"

/* --------------------------------------------------------------------- */
/*  Factorisation ------------------------------------------------------ */
/*  Produces up to 8 distinct primes (enough for n ≤ 10 000).              */

/* --------------------------------------------------------------------- */
/*  Divisor generation -------------------------------------------------- */
/*  Recursively builds all divisors into the array `out`.                 */
/* --------------------------------------------------------------------- */
static void gen_divs_recursive(size_t idx, seq_t cur,
                               const uint64_t *primes,
                               const int *exps,
                               size_t pcnt,
                               seq_t *out,
                               size_t *outcnt)
{
    if (idx == pcnt)
    {
        out[(*outcnt)++] = cur;
        return;
    }
    seq_t val = cur;
    /*  e = 0 … exps[idx]  */
    for (int e = 0; e <= exps[idx]; ++e)
    {
        gen_divs_recursive(idx + 1, val, primes, exps, pcnt, out, outcnt);
        val = val * (seq_t)primes[idx];
    }
}

/* --------------------------------------------------------------------- */
/*  Simple insertion sort of `divs[0..cnt-1]` -------------------------- */

/* --------------------------------------------------------------------- */
/*  Main sequence function ------------------------------------------------ */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* ---- 1. Factorise n ------------------------------------------------ */
    uint64_t primes[10];
    int exps[10];
    size_t pcnt;
    factorise_uint64((uint64_t)n, primes, exps, &pcnt);

    /* ---- 2. Generate all divisors ------------------------------------- */
    static seq_t divs[256];             /* n ≤ 10 000 ⇒ d(n) ≤ 128 */
    size_t divcnt = 0;
    if (pcnt == 0)                      /* n == 1 */
    {
        divs[0] = (seq_t)1;
        divcnt  = 1;
    }
    else
        gen_divs_recursive(0, (seq_t)1, primes, exps, pcnt, divs, &divcnt);

    /* ---- 3. Sort divisors --------------------------------------------- */
    sort_divs(divs, divcnt);

    /* ---- 4. Find maximum window size ---------------------------------- */
    const unsigned long long NUM = 2718281828459045ULL;    /* e * 10^15  */
    const unsigned long long DEN = 1000000000000000ULL;    /* 10^15      */
    seq_t best = 0;
    size_t j = 0;
    for (size_t i = 0; i < divcnt; ++i)
    {
        unsigned __int128 prod = (unsigned __int128)((uint64_t)divs[i]) * NUM;
        seq_t upper = (seq_t)(prod / DEN);

        while (j < divcnt && divs[j] <= upper)
            ++j;

        seq_t count = (seq_t)(j - i);
        if (count > best)
            best = count;
    }

    return best;
}
