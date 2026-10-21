/*
 * A205452  Least s(k) such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=Fibonacci(2j).
 * https://oeis.org/A205452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the definition of OEIS sequence A205452.
   a(n) is the least s(k) such that n divides s(k)-s(j) for some j<k,
   where s(j)=Fibonacci(2j).  The value returned is s(k)=F_{2k}.  */
/* The algorithm generates Fibonacci numbers F_i and their residues
   modulo n until a residue repeats among the even‑indexed terms
   (i=2j).  By pigeonhole principle the first repetition yields
   the desired a(n).  Each call is independent; an internal cache
   is used only to store seen residues for the current n.  */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial or unexpected inputs */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for residues, allocated once and reused */
    static int *seen = NULL;
    static idx_t seen_size = 0;

    /* Allocate or resize the residue table if needed */
    if (n > seen_size) {
        int *tmp = (int *)realloc(seen,
                                  (size_t)n * sizeof(int));
        if (tmp == NULL)          /* allocation failure – return 0 */
            return (seq_t)0;
        seen = tmp;
        seen_size = n;
    }
    /* Initialise the table: -1 means residue not yet seen */
    for (idx_t i = 0; i < n; ++i)
        seen[i] = -1;

    /* Fibonacci state: F_0=0, F_1=1 */
    seq_t Fprev = (seq_t)0;           /* F_(i-1) */
    seq_t Fcurr = (seq_t)1;           /* F_i   */
    idx_t mod_prev = (idx_t)0;        /* F_(i-1) mod n */
    idx_t mod_curr = (idx_t)(1 % n);  /* F_i   mod n */
    idx_t i = 1;                      /* current Fibonacci index i */

    /* Iterate until a repeat is found among even indices */
    while (1) {
        /* Compute next Fibonacci number and its residue modulo n */
        seq_t nextF = Fprev + Fcurr;
        idx_t nextMod = mod_prev + mod_curr;
        if (nextMod >= n)
            nextMod -= n;            /* fast modulo (n <= UINT64_MAX) */

        /* Shift to next state */
        Fprev = Fcurr;
        Fcurr = nextF;
        mod_prev = mod_curr;
        mod_curr = nextMod;
        ++i;                         /* now i points to F_i = Fcurr */

        /* Only even indices (i = 2j) correspond to s(j) values */
        if ((i & 1) == 0) {
            if (seen[mod_curr] != -1) {
                /* First repetition of residue: return the current s(i/2) */
                return Fcurr;
            }
            /* Record that this residue has appeared */
            seen[mod_curr] = (int)(i >> 1);  /* i/2 is the j index, value unused */
        }
    }
}
