/*
 * A018844  Arises from generalized Lucas-Lehmer test for primality.
 * https://oeis.org/A018844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A018844
 *  Sequence: Union of the recurrences
 *      a_1 = 4,   a_2 = 52,   a_n   = 14*a_{n-1} - a_{n-2}
 *      b_1 =10,   b_2 =970,   b_n   = 98*b_{n-1} - b_{n-2}
 *  The sequence is the sorted multiset of all a_n and b_n,
 *  with duplicates removed (a when a_n == b_m, only one copy is taken).
 *  The implementation keeps two growing vectors (A and B) with the
 *  recurrence results and a cache of the sorted union.  Each call
 *  to a(n) computes all terms up to n and returns the n‑th value.
 *  Arrow:
 *      a(1) = 4,  a(2) = 10,  a(3) = 52,  a(4) = 724, ...
 */

#include <stdlib.h>

static seq_t *cache   = NULL;
static size_t cache_cap = 0, cache_len = 0;

static seq_t *A = NULL, *B = NULL;
static size_t A_cap = 0, A_len = 0;
static size_t B_cap = 0, B_len = 0;
static size_t A_idx = 0, B_idx = 0;

static void push_cache(seq_t v)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap << 1 : 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = v;
}

static void push_A(seq_t v)
{
    if (A_len == A_cap) {
        A_cap = A_cap ? A_cap << 1 : 1;
        A = (seq_t *)realloc(A, A_cap * sizeof(seq_t));
    }
    A[A_len++] = v;
}

static void push_B(seq_t v)
{
    if (B_len == B_cap) {
        B_cap = B_cap ? B_cap << 1 : 1;
        B = (seq_t *)realloc(B, B_cap * sizeof(seq_t));
    }
    B[B_len++] = v;
}

static void next_A(void)
{
    seq_t nxt = 14 * A[A_len - 1] - A[A_len - 2];
    push_A(nxt);
}

static void next_B(void)
{
    seq_t nxt = 98 * B[B_len - 1] - B[B_len - 2];
    push_B(nxt);
}

/* External function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n<1 is undefined */

    /* Initialise sequences on first call */
    if (cache_len == 0) {
        /* Allocate small initial capacity */
        A_cap = B_cap = 4;
        A = (seq_t *)malloc(A_cap * sizeof(seq_t));
        B = (seq_t *)malloc(B_cap * sizeof(seq_t));

        push_A((seq_t)4);
        push_A((seq_t)52);
        push_B((seq_t)10);
        push_B((seq_t)970);

        A_idx = 0;
        B_idx = 0;
    }

    /* Extend cached sequence until we have the n‑th term */
    while (cache_len < n) {
        /* Ensure next candidate values exist */
        if (A_idx >= A_len) next_A();
        if (B_idx >= B_len) next_B();

        seq_t a_val = A[A_idx];
        seq_t b_val = B[B_idx];

        if (a_val <= b_val) {
            push_cache(a_val);
            ++A_idx;
            if (a_val == b_val)           /* skip duplicate from B */
                ++B_idx;
        } else {
            push_cache(b_val);
            ++B_idx;
        }
    }

    return cache[n - 1];
}
