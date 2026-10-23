/*
 * A046595  Denominators of the 1/4-Pascal triangle (by row), excluding 1's.
 * https://oeis.org/A046595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) = denominators of the 1/4‑Pascal triangle (by row), *
 *            obtained by summing adjacent entries of the previous row, * with
 *            the first interior element (row 2, column 1) set to 1/4. *
 *            Values are returned as seq_t, excluding denominators equal to 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------------------------------------------*/
#ifdef __cplusplus
#include <vector>
#include <cstddef>
#else
#include <stddef.h>
#include <stdlib.h>
#endif

/* Node representing a fraction in reduced form:
   odd part of the numerator and exponent of 2 in the denominator. */
typedef struct {
    seq_t odd;   /* odd numerator */
    uint8_t exp; /* exponent of 2 in denominator */
} node_t;

/* Count trailing zeros of a seq_t value.  Works for integer types
 * and for arbitrary‑precision types supporting bitwise ops. */
static uint8_t cnt_trailing_zeros(seq_t x)
{
    uint8_t c = 0;
    while ((x & (seq_t)1) == 0) {
        ++c;
        x >>= 1;
    }
    return c;
}

/* Add two fractions represented by node_t.  Result is reduced. */
static node_t add_nodes(const node_t *A, const node_t *B)
{
    node_t R;
    if (A->exp == B->exp) {
        seq_t sum = A->odd + B->odd;
        uint8_t v = cnt_trailing_zeros(sum);
        if (A->exp > v)
            R.exp = A->exp - v;
        else
            R.exp = 0;
        R.odd = sum >> v;
    } else {
        const node_t *small = (A->exp < B->exp) ? A : B;
        const node_t *large = (A->exp < B->exp) ? B : A;
        seq_t shift = small->odd << (large->exp - small->exp);
        seq_t sum   = shift + large->odd;
        R.exp = large->exp;
        R.odd = sum;
    }
    return R;
}

/*--------------------------------------------------------------------*/
/* Main sequence function                                              */
/*--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; this case is unused */

#ifdef __cplusplus
    /* C++ implementation using std::vector                                    */
    const seq_t one = (seq_t)1;
    std::vector<node_t> prev;          /* previous row */
    prev.reserve(2);
    prev.push_back({one, 0});          /* row 1: 1 1 */
    prev.push_back({one, 0});

    idx_t produced = 0;

    for (;;) {
        /* Special handling of row 2 (first interior element) */
        if (prev.size() == 2) {
            node_t mid = {one, 2};     /* 1/4  → exp = 2 */
            if (++produced == n)
                return one << mid.exp; /* 4 */
            /* Build row 2 and continue */
            prev.clear();
            prev.push_back({one, 0});
            prev.push_back(mid);
            prev.push_back({one, 0});
            continue;
        }

        /* General case: build next row from prev */
        size_t r = prev.size();       /* current row length */
        std::vector<node_t> next;
        next.reserve(r + 1);
        next.push_back({one, 0});      /* left edge */

        for (size_t i = 1; i < r; ++i) {
            node_t s = add_nodes(&prev[i - 1], &prev[i]);
            next.push_back(s);
            if (s.exp > 0) {
                if (++produced == n)
                    return one << s.exp;
            }
        }

        next.push_back({one, 0});      /* right edge */
        prev.swap(next);               /* next becomes current */
    }
#else
    /* C implementation using malloc/free                                    */
    const seq_t one = (seq_t)1;
    size_t prev_len = 2;              /* row 1 length */
    node_t *prev = (node_t *)malloc(prev_len * sizeof(node_t));
    prev[0] = (node_t){one, 0};
    prev[1] = (node_t){one, 0};

    idx_t produced = 0;

    for (;;) {
        /* Special handling of row 2 (first interior element) */
        if (prev_len == 2) {
            node_t mid = {one, 2};     /* 1/4 → exp = 2 */
            if (++produced == n) {
                free(prev);
                return one << mid.exp; /* 4 */
            }
            node_t *next = (node_t *)malloc(3 * sizeof(node_t));
            next[0] = (node_t){one, 0};
            next[1] = mid;
            next[2] = (node_t){one, 0};
            free(prev);
            prev = next;
            prev_len = 3;
            continue;
        }

        /* General case: build next row from prev */
        size_t next_len = prev_len + 1;
        node_t *next = (node_t *)malloc(next_len * sizeof(node_t));
        next[0] = (node_t){one, 0};                /* left edge */

        for (size_t i = 1; i < next_len - 1; ++i) {
            node_t s = add_nodes(&prev[i - 1], &prev[i]);
            next[i] = s;
            if (s.exp > 0) {
                if (++produced == n) {
                    free(prev);
                    free(next);
                    return one << s.exp;
                }
            }
        }

        next[next_len - 1] = (node_t){one, 0};       /* right edge */
        free(prev);
        prev = next;
        prev_len = next_len;
    }
#endif
}
