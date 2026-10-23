/*
 * A102471  Numbers n such that the denominator of Sum_{k=0 to 2n} 1/k! is
 *          (2n)!.
 * https://oeis.org/A102471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102471
 * Numbers n such that the denominator of Sum_{k=0 to 2n} 1/k! is (2n)!.
 *
 * The algorithm keeps the numerator N_m of the partial sum S_m=
 * sum_{k=0}^{2m} 1/k! written as N_m/(2m)!.  The recurrence
 *
 *     N_0 = 1
 *     N_{m+1} = (2m+1)(2m+2) * N_m + (2m+3)
 *
 * is used for each successive m.  For each prime p ≤ 2m+2 we maintain
 * N_m mod p.  A value m belongs to the sequence iff all these residues
 * are non‑zero, i.e. N_m is coprime to (2m)!.
 *
 * The implementation precomputes all primes up to a fixed upper bound
 * and updates the modular residues incrementally.  The array of valid
 * terms is built lazily; the nth term a(n) is returned by index n
 * (offset 1, so a(1) = 0).
 *
 * The routine works for seq_t of any width (byte, word, big integer)
 * and for idx_t as uint64_t.  The generation stops as soon as the
 * requested index n has been reached.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXPRIME 200000          /* primes up to this value are sufficient for n ≤ 10000 */

static int  primes[ MAXPRIME + 1 ];  /* storage for primes */
static int  primeCnt = 0;           /* number of primes found */

/* modulo residues for the current N_m, one per prime */
static uint32_t *mods = NULL;

/* list of valid terms (the m values belonging to the sequence) */
static seq_t *vals   = NULL;
static size_t valCnt = 0, valCap = 0;

/* current m that has been processed */
static idx_t curM = 0;

/* flag indicating that one‑time initialisation has been done */
static bool initDone = false;

/* ------------------------------------------------------------------ */
static void init_sequence(void)
{
    /* --- sieve for primes up to MAXPRIME -------------------------------- */
    bool *isComposite = (bool*)calloc(MAXPRIME + 1, sizeof(bool));
    for (int i = 2; i <= MAXPRIME; ++i) {
        if (!isComposite[i]) {
            primes[primeCnt++] = i;
            if ((int64_t)i * i <= MAXPRIME) {
                for (int64_t j = (int64_t)i * i; j <= MAXPRIME; j += i)
                    isComposite[(int)j] = true;
            }
        }
    }
    free(isComposite);

    /* --- allocate array of residues and initialise N_0 = 1 ----------------- */
    mods = (uint32_t*)malloc((size_t)primeCnt * sizeof(uint32_t));
    for (int i = 0; i < primeCnt; ++i)
        mods[i] = 1 % (uint32_t)primes[i];

    /* --- initialise list of valid terms: m = 0 is always valid ------------- */
    valCap = 64;
    vals   = (seq_t*)malloc(valCap * sizeof(seq_t));
    vals[0] = (seq_t)0;
    valCnt = 1;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initDone) {
        init_sequence();
        initDone = true;
    }

    /* grow the list until we have at least n terms */
    while (valCnt < n) {
        /* --- next candidate m ------------------------------------------ */
        idx_t mPrev = curM;
        curM = curM + 1;

        /* recurrence constants for N_{m} */
        uint64_t mul = (uint64_t)(2 * mPrev + 1) * (uint64_t)(2 * mPrev + 2);
        uint32_t add = (uint32_t)(2 * mPrev + 3);

        /* --- update residues N_m mod p for all primes ------------------- */
        for (int i = 0; i < primeCnt; ++i) {
            uint32_t p = (uint32_t)primes[i];
            uint32_t mul_mod = (uint32_t)(mul % p);
            uint32_t add_mod = add % p;
            mods[i] = (uint32_t)(((uint64_t)mul_mod * mods[i] + add_mod) % p);
        }

        /* --- check whether all residues are non‑zero up to 2*curM --------- */
        bool ok = true;
        int limit = (int)(2 * curM);
        for (int i = 0; i < primeCnt; ++i) {
            if (primes[i] > limit)
                break;
            if (mods[i] == 0) {
                ok = false;
                break;
            }
        }

        /* --- if ok, append m to the list ------------------------------- */
        if (ok) {
            if (valCnt == valCap) {
                valCap *= 2;
                vals = (seq_t*)realloc(vals, valCap * sizeof(seq_t));
            }
            vals[valCnt++] = (seq_t)curM;
        }
    }

    return vals[n - 1];
}
