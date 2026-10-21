/*
 * A225385  Construct sequences P,Q,R by the rules: Q = first differences of
 *          P, R = second differences of P, P starts with 1,3,9, Q starts with
 *          2,6, R starts with 4; at each stage the smallest number not yet
 *          present in P,Q,R is appended to R. Sequence gives P.
 * https://oeis.org/A225385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence is generated recursively. Start with P = {1,3,9}, Q
 *            = {2,6}, R = {4}. Repeatedly append to R the smallest positive
 *            integer not yet in P∪Q∪R. Then update Q and P by incremental
 *            differences: Q_new = Q_last + R_new P_new = P_last + Q_new
 *            Return the n‑th element of P
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global storage for the sequences and auxiliary data structures */
static seq_t *P = NULL;
static size_t P_len = 0, P_cap = 0;
static seq_t *Q = NULL;
static size_t Q_len = 0, Q_cap = 0;
static seq_t *R = NULL;
static size_t R_len = 0, R_cap = 0;

static uint8_t *seen_bits = NULL;    /* bitset: bit i is set if i has appeared */
static size_t seen_bits_bytes = 0;    /* number of bytes allocated in seen_bits */

static size_t next_candidate = 1;     /* smallest non‑negative integer not seen yet */

/* --- Helpers for dynamic arrays --- */
static void grow_P(size_t min_len)
{
    if (min_len <= P_cap) return;
    size_t new_cap = P_cap ? P_cap : 8;
    while (new_cap < min_len) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
    P = tmp; P_cap = new_cap;
}

static void grow_Q(size_t min_len)
{
    if (min_len <= Q_cap) return;
    size_t new_cap = Q_cap ? Q_cap : 8;
    while (new_cap < min_len) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(Q, new_cap * sizeof(seq_t));
    Q = tmp; Q_cap = new_cap;
}

static void grow_R(size_t min_len)
{
    if (min_len <= R_cap) return;
    size_t new_cap = R_cap ? R_cap : 8;
    while (new_cap < min_len) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(R, new_cap * sizeof(seq_t));
    R = tmp; R_cap = new_cap;
}

/* --- Helpers for the presence bitset --- */
static void ensure_seen(size_t idx)
{
    if (idx < seen_bits_bytes * 8) return;
    size_t needed_bytes = idx / 8 + 1;
    size_t new_bytes = seen_bits_bytes ? seen_bits_bytes * 2 : 8;
    while (new_bytes < needed_bytes) new_bytes <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(seen_bits, new_bytes);
    if (tmp) {
        /* initialize new part with zeros */
        for (size_t i = seen_bits_bytes; i < new_bytes; ++i) tmp[i] = 0;
        seen_bits = tmp;
    }
    seen_bits_bytes = new_bytes;
}

static inline bool seen_contains(seq_t val)
{
    size_t v = (size_t)val;
    if (v >= seen_bits_bytes * 8) return false;
    return (seen_bits[v >> 3] >> (v & 7)) & 1;
}

static inline void seen_set(seq_t val)
{
    size_t v = (size_t)val;
    ensure_seen(v);
    seen_bits[v >> 3] |= (uint8_t)(1u << (v & 7));
}

/* --- Initialization routine --- */
static void seq_init(void)
{
    if (P) return;                 /* already initialized */

    /* allocate initial capacities */
    grow_P(3);  grow_Q(2);  grow_R(1);
    seen_bits_bytes = 8;  seen_bits = (uint8_t *)calloc(seen_bits_bytes, 1);

    /* initial terms */
    P[0] = (seq_t)1;  P[1] = (seq_t)3;  P[2] = (seq_t)9;  P_len = 3;
    Q[0] = (seq_t)2;  Q[1] = (seq_t)6;  Q_len = 2;
    R[0] = (seq_t)4;  R_len = 1;

    /* mark them as seen */
    seen_set((seq_t)1); seen_set((seq_t)3); seen_set((seq_t)9);
    seen_set((seq_t)2); seen_set((seq_t)6);
    seen_set((seq_t)4);

    /* initial next_candidate */
    while (seen_contains((seq_t)next_candidate)) ++next_candidate;
}

/* --- Public sequence function --- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but never called */
    seq_init();

    size_t target = (size_t)n;
    while (P_len < target) {
        /* Step 1: choose next R value */
        seq_t R_val = (seq_t)next_candidate;
        seen_set(R_val);
        ++R_len;  grow_R(R_len);
        R[R_len-1] = R_val;

        /* Advance candidate to next missing integer */
        do { ++next_candidate; } while (seen_contains((seq_t)next_candidate));

        /* Step 2: compute new Q element */
        seq_t Q_last = Q[Q_len-1];
        seq_t Q_new = Q_last + R_val;
        seen_set(Q_new);
        ++Q_len;  grow_Q(Q_len);
        Q[Q_len-1] = Q_new;

        /* Step 3: compute new P element */
        seq_t P_last = P[P_len-1];
        seq_t P_new = P_last + Q_new;
        seen_set(P_new);
        ++P_len;  grow_P(P_len);
        P[P_len-1] = P_new;
    }

    return P[n-1];
}
