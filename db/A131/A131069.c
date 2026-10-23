/*
 * A131069  Numbers n such that n = sum_digits(a)^sum_digits(b) = a+b, with a
 *          and b positive integers.
 * https://oeis.org/A131069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer n such that there exist positive
 *            integers a,b with a+b=n and sumdigits(a)^sumdigits(b)=n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   compare_seq_t, seq_mul, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/seq_to_str.h"
#include "../utils/seq_mul.h"
#include "../utils/compare_seq_t.h"   /* provides compare_seq_t() */

/* --- DP routine ----------------------------------------------------- */

static bool exists_pair(seq_t n, unsigned s, unsigned t)
{
    /* trivial case for n == 1 (handled elsewhere) */
    if (n == (seq_t)0) return false;

    /* get decimal digits of n in reverse order */
    char buf[1024];
    size_t len_buf;
    seq_to_str(n, buf, &len_buf);
    unsigned len = (unsigned)len_buf;
    unsigned *digits = (unsigned *)malloc(len * sizeof(unsigned));
    for (size_t i = 0; i < len; ++i) digits[i] = buf[len - 1 - i] - '0';

    unsigned max_sum = 9u * len;
    if (s > max_sum || t > max_sum) {
        free(digits);
        return false;
    }

    unsigned dimA = s + 1;
    unsigned dimB = t + 1;
    size_t stateSize = 2u * dimA * dimB;

    unsigned char *dp  = (unsigned char *)calloc(stateSize, 1);
    unsigned char *ndp = (unsigned char *)malloc(stateSize);
    if (!dp || !ndp) { free(digits); free(dp); free(ndp); return false; }

    /* initial state: carry=0, sumA=0, sumB=0 */
    dp[0] = 1;

    for (unsigned pos = 0; pos < len; ++pos) {
        unsigned d = digits[pos];
        memset(ndp, 0, stateSize);

        for (unsigned carry = 0; carry < 2; ++carry) {
            for (unsigned sumA = 0; sumA <= s; ++sumA) {
                for (unsigned sumB = 0; sumB <= t; ++sumB) {
                    size_t idx = (carry * dimA + sumA) * dimB + sumB;
                    if (!dp[idx]) continue;

                    for (unsigned a = 0; a < 10; ++a) {
                        int temp = (int)d - (int)a - (int)carry;
                        if (temp < 0) temp += 10;
                        unsigned b = (unsigned)temp;
                        unsigned nextCarry = (a + b + carry) >= 10 ? 1 : 0;
                        unsigned newSumA = sumA + a;
                        unsigned newSumB = sumB + b;
                        if (newSumA > s || newSumB > t) continue;
                        size_t nidx = (nextCarry * dimA + newSumA) * dimB + newSumB;
                        ndp[nidx] = 1;
                    }
                }
            }
        }
        /* swap dp and ndp */
        unsigned char *tmp = dp; dp = ndp; ndp = tmp;
    }

    size_t finalIdx = (0 * dimA + s) * dimB + t;
    bool ok = dp[finalIdx] != 0;

    free(digits);
    free(dp);
    free(ndp);
    return ok;
}

/* --- Min–heap on seq_t ---------------------------------------------- */

typedef struct {
    seq_t val;
    unsigned base;
    unsigned exp;
} Node;

static int node_cmp(const Node *a, const Node *b)
{
    return compare_seq_t(&(a->val), &(b->val));
}

typedef struct {
    Node *data;
    size_t size;
    size_t cap;
} Heap;

static Heap heap = {NULL, 0, 0};

static void heap_init(void)
{
    heap.cap = 1024;
    heap.data = (Node *)malloc(heap.cap * sizeof(Node));
    heap.size = 0;

    const unsigned MAX_BASE = 32768; /* adequate for 10 000 terms */
    for (unsigned b = 2; b <= MAX_BASE; ++b) {
        Node n;
        n.base = b;
        n.exp  = 2;
        n.val  = seq_mul((seq_t)b, (seq_t)b);
        /* push */
        if (heap.size == heap.cap) {
            heap.cap *= 2;
            heap.data = (Node *)realloc(heap.data, heap.cap * sizeof(Node));
        }
        heap.data[heap.size++] = n;
        size_t i = heap.size - 1;
        while (i > 0) {
            size_t p = (i - 1) >> 1;
            if (node_cmp(&heap.data[i], &heap.data[p]) >= 0) break;
            Node tmp = heap.data[i]; heap.data[i] = heap.data[p]; heap.data[p] = tmp;
            i = p;
        }
    }
}

static Node heap_pop(void)
{
    Node ret = heap.data[0];
    heap.data[0] = heap.data[--heap.size];
    size_t i = 0;
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < heap.size && node_cmp(&heap.data[left], &heap.data[smallest]) < 0)
            smallest = left;
        if (right < heap.size && node_cmp(&heap.data[right], &heap.data[smallest]) < 0)
            smallest = right;
        if (smallest == i) break;
        Node tmp = heap.data[i]; heap.data[i] = heap.data[smallest]; heap.data[smallest] = tmp;
        i = smallest;
    }
    return ret;
}

static void heap_push(Node n)
{
    if (heap.size == heap.cap) {
        heap.cap *= 2;
        heap.data = (Node *)realloc(heap.data, heap.cap * sizeof(Node));
    }
    heap.data[heap.size++] = n;
    size_t i = heap.size - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (node_cmp(&heap.data[i], &heap.data[p]) >= 0) break;
        Node tmp = heap.data[i]; heap.data[i] = heap.data[p]; heap.data[p] = tmp;
        i = p;
    }
}

/* --- Sequence generation ---------------------------------------------- */

static seq_t *cache = NULL;
static size_t cache_len = 0;

static void generate_up_to(idx_t target)
{
    while (cache_len < target) {
        Node cur = heap_pop();
        seq_t nval = cur.val;
        unsigned s = cur.base;
        unsigned t = cur.exp;

        /* skip duplicate values */
        if (cache_len == 0 || compare_seq_t(&nval, &cache[cache_len - 1]) != 0) {
            if (exists_pair(nval, s, t)) {
                seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cache[cache_len++] = nval;
            }
        }

        /* push next power for this base */
        seq_t next_val = seq_mul(nval, (seq_t)s);
        Node nxt = { next_val, s, t + 1 };
        heap_push(nxt);
    }
}

/* --- External function ------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* lazily initialise */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;     /* a(1) = 1 */
        cache_len = 1;
        heap_init();
    }
    if (cache_len < n) {
        generate_up_to(n);
    }
    return cache[n - 1];
}
