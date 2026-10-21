/*
 * A099473  Numbers k such that binomial(2*k,k) cannot be represented as the
 *          sum of three squares.
 * https://oeis.org/A099473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 8256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A099473 – Numbers k such that the central binomial coefficient
    C(2k,k) cannot be expressed as the sum of three squares.
    Legendre's three‑square theorem says an integer N is a
    sum of three squares iff N ≠ 4^a(8b+7).  For the central
    binomial coefficient we have two simple criteria:

        1. v₂(C(2k,k)) = popcount(k).  Thus v₂ is even ⇔ k has even
           popcount.  If v₂ is odd the coefficient is always a sum
           of three squares.

        2. Let F[n] = ∏_{t=1..n} odd(t)  (mod 8), where odd(t)
           is the odd part of t (t >> ctz(t)).  Then
           odd_part(C(2k,k)) ≡ F[2k]·F[k]²  (mod 8).

    Hence C(2k,k) is NOT a sum of three squares precisely when
    (popcount(k) is even) AND (F[2k]·F[k]² ≡ 7 mod 8).  The routine
    below pre‑computes the F array incrementally and generates the
    sequence in O(1) time per term using only small auxiliary
    storage.  It works for any seq_t and idx_t defined by the build. */

#include <stdlib.h>
#include <stdint.h>

/* ---------------------------------------------------------------------- */
/* Helper: popcount for 64‑bit integers.  seq_t may be larger but idx_t
   is uint64_t.  This works for all builds. */
static inline unsigned int popcount_u64(uint64_t x) {
    return (unsigned int)__builtin_popcountll(x);
}

/* ---------------------------------------------------------------------- */
/* Storage for the prefix product F and the cached sequence terms. */
static uint8_t *F = NULL;          /* F[i] = product_{t=1..i} odd(t) mod 8  */
static size_t Fsize = 0, Fcap = 0;
static seq_t *terms = NULL;        /* cached k values that satisfy the test   */
static size_t termsize = 0, termcap = 0;
static idx_t lastk = 0;            /* smallest k that has been examined      */

/* ---------------------------------------------------------------------- */
/* Ensure that F contains entries up to n (inclusive).  We keep the
   product modulo 8;  since 8 fits in a byte the array stays tiny.
   The function reallocates with explicit casts so it compiles as
   both C and C++. */
static void ensure_F(size_t n) {
    if (n < Fsize) return;                 /* already ready */

    size_t oldsize = Fsize;
    size_t newsize = n + 1;                /* we need indices 0..n */

    /* grow capacity exponentially */
    size_t newcap = Fcap ? Fcap : 16;
    while (newcap < newsize) newcap <<= 1;

    /* realloc with explicit cast for C++ */
    void *tmp = realloc((void*)F, newcap);
    if (!tmp) abort();
    F = (uint8_t*)tmp;
    Fcap = newcap;

    /* initialise new entries */
    for (size_t i = oldsize; i < newsize; ++i) {
        if (i == 0) {              /* F[0] defined as 1 by convention */
            F[0] = 1;              /* product over empty set */
            continue;
        }
        /* odd part of i :  remove all trailing zeros by shifting */
        uint64_t odd = ((uint64_t)i) >> __builtin_ctzll((unsigned long long)i);
        uint8_t prev = F[i - 1];
        F[i] = (uint8_t)((prev * (odd & 7u)) & 7u);   /* modulo 8 */
    }

    Fsize = newsize;
}

/* ---------------------------------------------------------------------- */
/* Ensure the terms array can hold at least n elements.  Realloc via
   explicit cast for C++. */
static void ensure_terms(size_t n) {
    if (n <= termcap) return;
    size_t newcap = termcap ? termcap : 16;
    while (newcap < n) newcap <<= 1;

    void *tmp = realloc((void*)terms, newcap * sizeof(seq_t));
    if (!tmp) abort();
    terms = (seq_t*)tmp;
    termcap = newcap;
}

/* ---------------------------------------------------------------------- */
/* Main sequence function.  It lazily generates terms until the
   requested index is available.  The sequence is zero‑based in the
   table but the function uses the OEIS offset of 1. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* outside the defined offset */

    /* generate new terms until we have at least n of them */
    while (termsize < (size_t)n) {
        ++lastk;                                       /* test next k */

        /* F is needed up to 2*lastk */
        ensure_F((size_t)2 * (size_t)lastk);

        unsigned int e = popcount_u64((uint64_t)lastk);
        if ((e & 1U) == 0) {                        /* popcount even? */
            uint8_t odd  = F[(size_t)2 * (size_t)lastk];
            uint8_t fk   = F[(size_t)lastk];
            /* compute odd_part(C(2k,k)) mod 8 :  odd * fk² */
            uint8_t t = odd;
            t = (t * fk) & 7U;    /* multiply by fk once */
            t = (t * fk) & 7U;    /* multiply by fk again */

            if (t == 7U) {                     /* 4^a(8b+7) condition */
                ensure_terms(termsize + 1);
                terms[termsize++] = (seq_t)lastk;
            }
        }
    }

    return terms[(size_t)n - 1];
}
