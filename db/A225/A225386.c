/*
 * A225386  Construct sequences P,Q,R by the rules: Q = first differences of
 *          P, R = second differences of P, P starts with 1,3,9, Q starts with
 *          2,6, R starts with 4; at each stage the smallest number not yet
 *          present in P,Q,R is appended to R. Sequence gives Q.
 * https://oeis.org/A225386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A225386
 *  The value a(n) is the n‑th element of the sequence Q produced by
 *  iterating the construction described in the OEIS entry:
 *
 *    P starts 1,3,9.   Q is the first differences of P (initially 2,6).
 *    R is the second differences of P (initially 4).
 *    At each step, append to R the smallest integer not yet present
 *    in P, Q or R.  This R creates a new Q entry (previous Q + R)
 *    which creates a new P entry (previous P + new Q).  The sequence
 *    returned by this function is the Q sequence.
 *
 *  The implementation uses dynamic arrays for P and Q and a bitset
 *  (uint64_t vector) to keep track of numbers that have already been
 *  used.  It memoises across calls so that an arbitrary order of
 *  requests for a(n) is supported efficiently.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static seq_t *P      = NULL;
static size_t p_len = 0, p_cap = 0;

static seq_t *Q      = NULL;
static size_t q_len = 0, q_cap = 0;

/* bitset: one 64‑bit word per 64 integers */
static uint64_t *used_bits = NULL;
static size_t used_words  = 0;

static seq_t candidate = 1;   /* smallest integer not yet tested */
static bool initialized = false;

/* grow the bitset so that bit v can be accessed */
static void grow_used(uint64_t v)
{
    size_t new_words = used_words ? used_words : 1;
    while (v >= new_words * 64ULL) new_words <<= 1;

    uint64_t *new_bits = (uint64_t *)calloc(new_words, sizeof(uint64_t));
    if (!new_bits) exit(EXIT_FAILURE);

    if (used_bits) {
        memcpy(new_bits, used_bits, used_words * sizeof(uint64_t));
        free(used_bits);
    }
    used_bits  = new_bits;
    used_words = new_words;
}

/* test presence of a value in the bitset */
static bool used_has(seq_t v)
{
    uint64_t x = (uint64_t)v;
    if (x >= used_words * 64ULL) return false;
    return (used_bits[x >> 6] & (1ULL << (x & 63ULL))) != 0;
}

/* add a value to the bitset */
static void used_add(seq_t v)
{
    uint64_t x = (uint64_t)v;
    if (x >= used_words * 64ULL) grow_used(x);
    used_bits[x >> 6] |= 1ULL << (x & 63ULL);
}

/* dynamic array append helper */
static inline void append_seq(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
        seq_t *p = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!p) exit(EXIT_FAILURE);
        *arr = p;
        *cap = newcap;
    }
    (*arr)[(*len)++] = val;
}

static inline void append_P(seq_t v) { append_seq(&P, &p_len, &p_cap, v); }
static inline void append_Q(seq_t v) { append_seq(&Q, &q_len, &q_cap, v); }

/* initialise base sequences and used set */
static void init_once(void)
{
    if (initialized) return;
    initialized = true;

    /* P: 1,3,9 */
    p_cap = 4;
    P     = (seq_t *)malloc(p_cap * sizeof(seq_t));
    if (!P) exit(EXIT_FAILURE);
    p_len = 3;
    P[0] = (seq_t)1;
    P[1] = (seq_t)3;
    P[2] = (seq_t)9;

    /* Q: 2,6 */
    q_cap = 4;
    Q     = (seq_t *)malloc(q_cap * sizeof(seq_t));
    if (!Q) exit(EXIT_FAILURE);
    q_len = 2;
    Q[0] = (seq_t)2;
    Q[1] = (seq_t)6;

    /* used set initial values: P[0],P[1],P[2],Q[0],Q[1] and first R=4 */
    used_words = 0;
    grow_used(64);
    used_add((seq_t)1);
    used_add((seq_t)3);
    used_add((seq_t)9);
    used_add((seq_t)2);
    used_add((seq_t)6);
    used_add((seq_t)4);
}

/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence offset is 1 */
    init_once();

    /* extend until we have n terms of Q */
    while (q_len < (size_t)n) {
        /* find the smallest unused integer */
        while (used_has(candidate)) candidate++;

        seq_t r = candidate;
        used_add(r);
        candidate++;

        /* create new Q entry */
        seq_t Q_last = Q[q_len - 1];
        seq_t Q_new  = Q_last + r;
        append_Q(Q_new);
        used_add(Q_new);

        /* create new P entry */
        seq_t P_last = P[p_len - 1];
        seq_t P_new  = P_last + Q_new;
        append_P(P_new);
        used_add(P_new);
    }

    return Q[n - 1];
}
