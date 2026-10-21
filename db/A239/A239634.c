/*
 * A239634  Initial digits of semiprimes in decimal representation.
 * https://oeis.org/A239634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the leading decimal digit of the n‑th semiprime
   (i.e., a(n) = A000030(A001358(n))).  The function expands a
   simple sieve dynamically until it has generated at least n
   semiprimes, then extracts and returns the first digit.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- static state for the incremental sieve ------------------- */
static uint32_t *spf      = NULL;   /* smallest prime factor array          */
static uint64_t *sem_pr   = NULL;   /* generated semiprimes (stored as 64‑bit) */
static size_t   sem_n     = 0;   /* number of semiprimes stored          */
static idx_t    limit     = 0;   /* current sieve limit                  */

/* Re‑sieve up to a limit that produces at least 'need' semiprimes. */
static void ensure_cache(idx_t need)
{
    if (need <= sem_n)
        return;               /* already have enough */

    if (limit == 0)
        limit = 200000;       /* initial guess for n up to ~10 000 */

    while (true) {
        /* Allocate and initialise SPF array */
        free(spf);
        spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
        if (!spf)
            return;           /* OOM; caller will get 0 */

        for (idx_t i = 0; i <= limit; ++i)
            spf[i] = (uint32_t)i;

        /* Classic SPF sieve */
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (spf[i] == (uint32_t)i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == (uint32_t)j)
                        spf[j] = (uint32_t)i;
            }
        }

        /* Gather semiprimes up to current limit */
        free(sem_pr);
        sem_pr = (uint64_t*)malloc(limit * sizeof(uint64_t));
        if (!sem_pr)
            return;           /* OOM */

        size_t cnt = 0;
        for (idx_t x = 4; x <= limit; ++x) {
            uint32_t p = spf[x];
            if (p == x)          /* prime – skip */
                continue;
            uint32_t q = (uint32_t)(x / p);
            if (q >= 2 && spf[q] == q)  /* both factors are prime */
                sem_pr[cnt++] = (uint64_t)x;
            if (cnt >= need)                     /* enough */
                break;
        }

        sem_n = cnt;
        if (sem_n >= need)
            break;      /* satisfied request */

        /* Need more primes – double the limit and retry */
        limit <<= 1;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1; return 0 to keep type consistent */

    ensure_cache(n);

    uint64_t v = sem_pr[n - 1];   /* nth semiprime (0‑based index) */

    /* Extract the most significant decimal digit */
    while (v >= 10)
        v /= 10;

    return (seq_t)v;   /* v is 0..9, always fits in seq_t */
}
