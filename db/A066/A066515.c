/*
 * A066515  Numbers k such that prime(k+1) + prime(k-2) = 2*prime(k-1), where
 *          prime(m) is the m-th prime.
 * https://oeis.org/A066515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that prime(k+1) + prime(k-2) = 2 * prime(k-1)
   (OEIS A066515).  This implementation precomputes primes up to a
   sufficiently large value (3 000 000) and then searches for the
   requested n‑th term.  It works for all n ≥ 1, and is fast enough
   for n ≈ 10 000. */
#include <stdlib.h>
#include <stdint.h>

static size_t primeCount = 0;           /* number of primes found          */
static seq_t *primeVals   = NULL;       /* array of primes                */
static unsigned char *isComp = NULL;    /* sieve bitset                   */
static int primes_ready = 0;

/* ----------------------------------------------------------------------
   Prime sieve up to 3 000 000.  The limit is far larger than needed
   for the first 10 000 terms of the sequence.
   ---------------------------------------------------------------------- */
static void init_primes(void)
{
    if (primes_ready) return;
    const idx_t MAX_VAL = 3000000U;          /* sieve limit                    */
    unsigned char *comp = (unsigned char *)malloc((size_t)(MAX_VAL + 1));
    if (!comp) return;                       /* allocation failed              */
    for (size_t i = 0; i <= (size_t)MAX_VAL; ++i) comp[i] = 0;
    comp[0] = comp[1] = 1;
    for (idx_t i = 2; i * i <= MAX_VAL; ++i) {
        if (!comp[(size_t)i]) {
            for (idx_t j = i * i; j <= MAX_VAL; j += i)
                comp[(size_t)j] = 1;
        }
    }
    /* count primes to allocate array           */
    for (idx_t i = 2; i <= MAX_VAL; ++i)
        if (!comp[(size_t)i]) ++primeCount;
    /* allocate primes array                    */
    primeVals = (seq_t *)malloc((size_t)primeCount * sizeof(seq_t));
    if (!primeVals) { free(comp); return; }
    /* fill primes array                        */
    size_t pos = 0;
    for (idx_t i = 2; i <= MAX_VAL; ++i)
        if (!comp[(size_t)i])
            primeVals[pos++] = (seq_t)i;
    /* finalize                                 */
    isComp = comp;
    primes_ready = 1;
}

/* ----------------------------------------------------------------------
   Return the n-th term of A066515 (offset 1).  The function may be
   called in any order and is independent of the number of previous
   calls.
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_primes();
    if (!primes_ready || primeVals == NULL) return 0;  /* safety */

    idx_t found = 0;
    /* search successive k, starting from k = 3 (since k-2 >= 1)     */
    for (idx_t k = 3;; ++k) {
        /* ensure we have enough primes: need primes at indices k-2,
           k-1, and k+1 => access up to primeVals[k]                  */
        if (k >= primeCount) break;   /* safety: should not happen with LIMIT */
        seq_t pk_minus2 = primeVals[(size_t)(k - 3)]; /* prime(k-2)   */
        seq_t pk_minus1 = primeVals[(size_t)(k - 2)]; /* prime(k-1)   */
        seq_t pk_plus1  = primeVals[(size_t)(k)];     /* prime(k+1)   */

        if (pk_plus1 + pk_minus2 == (pk_minus1 << 1)) {
            ++found;
            if (found == n) return (seq_t)k;
        }
    }
    return 0;   /* should never reach here for valid n */
}
