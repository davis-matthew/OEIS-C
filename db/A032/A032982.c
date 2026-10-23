/*
 * A032982  Numbers with the property that all pairs of consecutive base-5
 *          digits differ by more than 2.
 * https://oeis.org/A032982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A032982 sequence:
 * Numbers with the property that all pairs of consecutive base‑5 digits
 * differ by more than 2.  The function performs a breadth‑first search
 * over admissible digit strings, yielding the numbers in increasing
 * order.  Implementation is valid C11 and C++17 alike. */

#include <stdlib.h>
#if defined(__cplusplus)
#include <new>
#endif

/* seq_t and idx_t are defined by the build system. */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* Pre‑computed adjacency for last base‑5 digit d:
 * next_count[d] = number of admissible following digits
 * next_digits[d][k] holds those digits. */
static const uint8_t next_count[5] = {2, 1, 0, 1, 2};
static const uint8_t next_digits[5][2] = {
    {3, 4},        /* d = 0 -> 3,4 */
    {4, 0},        /* d = 1 -> 4   */
    {0, 0},        /* d = 2 -> none*/
    {0, 0},        /* d = 3 -> 0   */
    {0, 1}         /* d = 4 -> 0,1 */
};

struct node {
    seq_t val;   /* current number in decimal */
    uint8_t last;/* last base‑5 digit */
};

/* Helper: construct or assign a seq_t value, depending on the language */
static inline void set_seq_val(seq_t *p, seq_t v)
{
#if defined(__cplusplus)
    new (p) seq_t(v);
#else
    *p = v;
#endif
}

seq_t a(idx_t n)
{
    /* Initial queue capacity (will grow). */
    size_t capacity = 16;
    size_t qsize = 0;     /* current number of nodes in queue    */
    size_t front = 0;     /* index of next node to pop           */
    struct node *q = (struct node *)malloc(capacity * sizeof(*q));
    if (!q) return (seq_t)0; /* allocation failure */

    /* Macro to push a new node onto the queue. */
    #define PUSH(v, l) do {                                           \
        if (qsize == capacity) {                                      \
            capacity <<= 1;                                            \
            struct node *newq = (struct node *)realloc(q, capacity * sizeof(*q)); \
            if (!newq) { free(q); return (seq_t)0; }                  \
            q = newq;                                                 \
        }                                                              \
        set_seq_val(&q[qsize].val, (v));                               \
        q[qsize].last = (l);                                          \
        ++qsize;                                                      \
    } while (0)

    /* Initialize queue with single‑digit valid numbers. */
    PUSH((seq_t)1, 1);
    PUSH((seq_t)2, 2);
    PUSH((seq_t)3, 3);
    PUSH((seq_t)4, 4);

    size_t count = 0;   /* how many terms produced */

    while (front < qsize) {
        struct node cur = q[front++];
        ++count;
        if (count == n) {
            /* Clean up allocated memory. */
            free(q);
            return cur.val;
        }

        uint8_t d = cur.last;
        uint8_t c = next_count[d];
        for (uint8_t i = 0; i < c; ++i) {
            uint8_t nd = next_digits[d][i];
            seq_t new_val = cur.val * (seq_t)5 + (seq_t)nd;
            PUSH(new_val, nd);
        }
    }

    /* Should never reach here for valid n. */
    free(q);
    return (seq_t)0;
}
