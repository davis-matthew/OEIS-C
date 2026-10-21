/*
 * A205554  Least positive integer k such that n divides k^(k-1)-j^(j-1) for
 *          some j in [1,k-1].
 * https://oeis.org/A205554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A205554:  least positive integer k such that
 *  n divides k^(k-1) - j^(j-1) for some 1 ≤ j < k.
 *  The function searches k from 2 upward; it stops the first time a
 *  residue k^(k-1) (mod n) repeats an earlier residue (including the
 *  residue from j = 1, namely 1^(1-1) = 1).  At most n+1 values are
 *  examined because only n residues modulo n exist.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* modular exponentiation: base^exp mod mod.
 *   exp is an idx_t; base and mod are seq_t.
 *   All intermediate values are kept below mod, so 64‑bit multiplication
 *   is safe when mod ≤ UINT64_MAX. */
static inline seq_t mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (seq_t)(((uint64_t)result * b) % mod);
        b = (seq_t)(((uint64_t)b * b) % mod);
        exp >>= 1;
    }
    return result;
}

/* Static buffer reused for all calls – grows as needed. */
static unsigned char *seen_buf = NULL;  /* 0 => unseen, non‑0 => seen */
static size_t seen_cap       = 0;       /* current capacity */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but return 0 safely */
        return 0;

    /* Allocate/resize the buffer to hold residues 0 … n-1 */
    if (seen_cap < (size_t)n) {
        free(seen_buf);
        seen_buf = (unsigned char*)malloc((size_t)n * sizeof(unsigned char));
        seen_cap = (size_t)n;
        if (!seen_buf)   /* allocation failure */
            return 0;
    }

    /* Mark all residues as unseen */
    memset(seen_buf, 0, seen_cap);

    /* Record the residue from j = 1 : 1^(1-1) = 1 (mod n) */
    size_t r1 = (size_t)(1 % (seq_t)n);
    seen_buf[r1] = 1;

    /* Search for the first k ≥ 2 with a repeated residue */
    for (idx_t k = 2; k <= n + 1; ++k) {
        seq_t rr = mod_pow((seq_t)k, k - 1, (seq_t)n);  /* k^(k-1) mod n */
        size_t idx = (size_t)rr;
        if (seen_buf[idx])            /* residue already seen */
            return (seq_t)k;          /* minimal k found */
        seen_buf[idx] = 1;            /* mark as seen */
    }

    /* Theoretically unreachable – but return something safe */
    return (seq_t)(n + 2);
}
