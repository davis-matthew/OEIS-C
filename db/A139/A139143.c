/*
 * A139143  This sequence and A139142 are complements. a(1)=2. A139142(1)=1.
 *          A139142(n+1) = A139142(n) + sum{k=1 to n} a(k).
 * https://oeis.org/A139143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A139143: complement of A139142.
 *  a(1) = 2;  b(1) = 1;
 *  For n ≥ 1:  b(n+1) = b(n) + Σ_{k=1}^{n} a(k).
 *  a(n) is the smallest positive integer not in the set {b(m)}.  */
#include <stddef.h>
#include <stdlib.h>

/* The build supplies these types. */
extern seq_t a(idx_t n);

/* ----------------------------------------------------------------------- */
/*  Internal static state                                                */
/* ----------------------------------------------------------------------- */
static seq_t *aSeq      = NULL;  /* a[n]  (1‑based, stored 0‑based)   */
static idx_t   aCnt     = 0;     /* number of a terms generated      */
static idx_t   aCap     = 0;     /* capacity of aSeq                 */

static seq_t   sumA = 0;          /* Σ a(1..current)                  */
static seq_t   b_cur = 0;         /* current b value (b(n))           */
static idx_t   nextCand = 0;      /* next integer to test as a(n)     */

static unsigned char *bitset = NULL; /* membership for b values   */
static idx_t   bitsSize = 0;        /* bitset size in bits          */

/* ----------------------------------------------------------------------- */
/*  Bitset helpers                                                       */
/* ----------------------------------------------------------------------- */
static void ensure_bitset(idx_t needBits) {
    if (needBits < bitsSize) return;
    idx_t newSize = bitsSize ? bitsSize << 1 : 64;
    while (newSize <= needBits) newSize <<= 1;
    idx_t newBytes = (newSize + 7) / 8;
    idx_t curBytes = (bitsSize + 7) / 8;
    unsigned char *tmp = (unsigned char*)realloc(bitset, newBytes);
    if (!tmp) abort();
    if (newBytes > curBytes) {
        /* zero the newly allocated trailing bytes */
        memset(tmp + curBytes, 0, newBytes - curBytes);
    }
    bitset = tmp;
    bitsSize = newSize;
}

static inline int has_bit(idx_t v) {
    return (bitset[v >> 3] >> (v & 7)) & 1;
}

static inline void set_bit(idx_t v) {
    ensure_bitset(v);
    bitset[v >> 3] |= (unsigned char)(1u << (v & 7));
}

/* ----------------------------------------------------------------------- */
/*  Dynamic array helpers                                                */
/* ----------------------------------------------------------------------- */
static void ensure_a(idx_t need) {
    if (need <= aCap) return;
    idx_t newCap = aCap ? aCap << 1 : 2;
    while (newCap < need) newCap <<= 1;
    seq_t *tmp = (seq_t*)realloc(aSeq, newCap * sizeof(seq_t));
    if (!tmp) abort();
    aSeq = tmp;
    aCap = newCap;
}

/* ----------------------------------------------------------------------- */
/*  Generate a[1..n]                                                    */
/* ----------------------------------------------------------------------- */
static void generate_a_up_to(idx_t n) {
    if (n == 0) return;

    /* First time: initialize the sequence. */
    if (aCnt == 0) {
        ensure_a(2);
        aSeq[0] = (seq_t)2;          /* a(1) = 2 */
        aCnt = 1;

        sumA = aSeq[0];              /* sum of a(1) */
        b_cur = (seq_t)1;            /* b(1) = 1 */

        /* initialise bitset for b values */
        set_bit((idx_t)b_cur);        /* mark 1 */

        /* compute b(2) before generating a(2) */
        seq_t b_next = b_cur + sumA;  /* 1 + 2 = 3 */
        b_cur = b_next;
        set_bit((idx_t)b_next);       /* mark 3 */

        nextCand = 4;                 /* next integer to try for a(n) */
    }

    /* Generate until we have aCnt >= n */
    while (aCnt < n) {
        /* Find smallest integer not already in B. */
        idx_t cand = nextCand;
        while (has_bit(cand)) ++cand;

        /* Record a(n) */
        ensure_a(aCnt + 1);
        aSeq[aCnt] = (seq_t)cand;
        sumA += aSeq[aCnt];
        aCnt++;

        /* Compute next b value (b(n+1)) */
        seq_t b_next = b_cur + sumA;
        b_cur = b_next;
        set_bit((idx_t)b_next);

        /* Next candidate for a will be after this number. */
        nextCand = cand + 1;
    }
}

/* ----------------------------------------------------------------------- */
/*  Public function                                                      */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* undefined for n = 0 */
    generate_a_up_to(n);
    return aSeq[n - 1];
}
