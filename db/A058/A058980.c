/*
 * A058980  Positive numbers of form 4*k such that card(invphi(4*k)) = 0 (the
 *          nontotients congruent to 0 modulo 4).
 * https://oeis.org/A058980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 6109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A058980: a(n) = the n-th positive integer that is a multiple of 4
   and is not a value of Euler's totient function φ.
   Formula: a(n) = the n‑th 4‑multiple m such that m ∉ {φ(k) : k ≥ 1}. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* If the build supplies a different name for the 128‑bit type,
   include its header here.  Otherwise seq_t may be an arbitrary
   integer type.  No other typedefs are defined. */
 /* seq_t and idx_t are supplied by the build. */

static bool initialized = false;                /* have we done the sieve yet? */
static idx_t limit          = 0;                /* current sieve upper bound   */
static idx_t *phi          = NULL;              /* Euler totient values         */
static unsigned char *isTotient = NULL;         /* 1 if value occurs as φ       */

/* List of found non‑totient multiples of 4. */
static seq_t *nonTotient4   = NULL;             /* array of seq_t values           */
static size_t nt_len         = 0;               /* number of elements already set  */
static size_t nt_cap         = 0;               /* capacity of nonTotient4          */

/* ------------------------------------------------------------ */
/* Helper to grow the non‑totient list when needed.             */
static inline void push_nt(idx_t m)
{
    if (nt_len == nt_cap) {
        nt_cap = nt_cap == 0 ? 8 : nt_cap * 2;
        seq_t *tmp = (seq_t *)realloc(nonTotient4, nt_cap * sizeof(seq_t));
        if (!tmp) return;               /* out of memory; silently ignore */
        nonTotient4 = tmp;
    }
    nonTotient4[nt_len++] = (seq_t)m;
}

/* ------------------------------------------------------------ */
/* Ensure that all totients up to 'lim' have been computed and
   the non‑totient multiples of 4 up to that limit have been added
   to nonTotient4 list.                                            */
static void extend_sieve(idx_t lim)
{
    idx_t *new_phi      = NULL;
    unsigned char *new_t = NULL;
    idx_t i, p, j;

    /* Allocate new arrays. */
    new_phi = (idx_t *)malloc((lim + 1) * sizeof(idx_t));
    if (!new_phi) return;              /* if allocation fails, keep the old data */
    new_t   = (unsigned char *)calloc((lim + 1), 1);
    if (!new_t) {
        free(new_phi);
        return;
    }

    /* Initialise phi[i] = i. */
    for (i = 0; i <= lim; ++i) new_phi[i] = i;

    /* Compute φ by sieve. */
    for (p = 2; p <= lim; ++p)
        if (new_phi[p] == p)          /* p is prime */
            for (j = p; j <= lim; j += p)
                new_phi[j] -= new_phi[j] / p;

    /* Mark values that occur as φ. */
    for (i = 1; i <= lim; ++i)
        if (new_phi[i] <= lim)
            new_t[new_phi[i]] = 1;

    /* If the sieve is expanded, we must rebuild the non‑totient list
       from scratch (since we recomputed all φ).  Otherwise we could
       simply extend by checking the new range.  Here we rebuild: */
    if (limit == 0) {
        /* First time: initialize list. */
        nt_len = 0;
    } else {
        /* Ensure that the list currently contains numbers up to 'limit'. */
    }

    /* Populate non‑totient multiples of 4 up to 'lim'. */
    for (i = 4; i <= lim; i += 4)
        if (!new_t[i])
            push_nt(i);

    /* Replace old arrays. */
    if (phi)      free(phi);
    if (isTotient) free(isTotient);
    phi         = new_phi;
    isTotient   = new_t;
    limit       = lim;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* sequence is 1‑based; 0 is invalid */

    /* Initialise at the first call.  We target a modest limit that
       covers the required range for n up to about 10 000. */
    if (!initialized) {
        extend_sieve(1000000);          /* 1 000 000 is generous for n≤10 000 */
        initialized = true;
    }

    /* If the current list is not long enough, increase the sieve
       size until we have enough terms. */
    while (nt_len < n) {
        /* Double the sieve limit.  Safeguard against overflow. */
        idx_t new_limit = limit * 2;
        if (new_limit <= limit) {      /* overflow protection */
            new_limit = limit + 1000000;  /* fallback to large increment */
        }
        extend_sieve(new_limit);
    }

    /* Return the n‑th term (1‑based). */
    return nonTotient4[n - 1];
}
