/*
 * A236292  Number of distinct cyclic permutations of the partitions of n; see
 *          comments.
 * https://oeis.org/A236292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p⊢n} f(p), where for a partition p=[x1,…,xk] sorted
 *            (x1≤…≤xk), f(p) = 1 if all parts are equal, otherwise k.
 *            Equivalently a(n)=S[n]−σ(n)+τ(n), where S[n] = Σ_{p⊢n} (#parts
 *            of p), σ(n) = sum of divisors of n, τ(n) = number of divisors of
 *            n. (See comments in the OEIS entry for A236292.)
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 100, u64 n <= 354,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* a(1) is the first term; for n<1 the behaviour is undefined. */
    if (n < 1U)
        return (seq_t)0;

    /* Convert to size_t for indexing; this is safe for the test limits. */
    size_t N = (size_t)n;

    /* Dynamically allocate the arrays that will hold intermediate data.  */
/*  In C++ the arbitrary‑precision integer type `seq_t` must be
    constructed with `new`; in C we may use `malloc`.  */
#ifdef __cplusplus
    seq_t *P   = new seq_t[N + 1];
    seq_t *S   = new seq_t[N + 1];
    seq_t *tau = new seq_t[N + 1];
    seq_t *sig = new seq_t[N + 1];
#else
    seq_t *P   = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *S   = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *tau = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *sig = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!P || !S || !tau || !sig)
        return (seq_t)0;  /* allocation failure – return 0 for safety */
#endif

    /* Initialise all entries to 0.  The arbitrary‑precision type is
       default‑constructed by `new`, but we explicitly assign 0 to
       satisfy the C requirement. */
    for (size_t i = 0; i <= N; ++i) {
        P[i] = (seq_t)0;
        S[i] = (seq_t)0;
        tau[i] = (seq_t)0;
        sig[i] = (seq_t)0;
    }
    P[0] = (seq_t)1;   /* empty partition counts as one partition of 0 */

    /* Count partitions and the total number of parts in all partitions. */
    for (size_t part = 1; part <= N; ++part) {
        for (size_t sum = part; sum <= N; ++sum) {
            seq_t oldP = P[sum - part];
            seq_t oldS = S[sum - part];
            P[sum] += oldP;
            S[sum] += oldS + oldP;   /* each new partition has one extra part */
        }
    }

    /* Compute σ(n) and τ(n) by a divisor sieve. */
    for (size_t d = 1; d <= N; ++d) {
        for (size_t m = d; m <= N; m += d) {
            tau[m] += (seq_t)1;
            /* m/d fits in size_t; convert to seq_t for storage */
            sig[m] += (seq_t)(m / d);
        }
    }

    /* a(n) = S[n] – σ(n) + τ(n) */
    seq_t result = S[N] - sig[N] + tau[N];

    /* Free the temporary arrays. */
#ifdef __cplusplus
    delete[] P;
    delete[] S;
    delete[] tau;
    delete[] sig;
#else
    free(P);
    free(S);
    free(tau);
    free(sig);
#endif

    return result;
}
