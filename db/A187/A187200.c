/*
 * A187200  Parse the Fibonacci word A003849 into distinct phrases 0, 1, 00,
 *          10, 100, 1001, 01, 001, 010, ...; a(n) = length of n-th phrase.
 * https://oeis.org/A187200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n‑th distinct phrase obtained by parsing
 *            the infinite Fibonacci word A003849. The function implements the
 *            description “Parse the Fibonacci word into distinct phrases” and
 *            returns the length of the n‑th phrase
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static binary trie of the prefixes that
   have already been seen.  The function a() can be called in any
   order; phrases are parsed only once and cached.  The Fibonacci word
   is pre‑generated up to 1 048 576 bits, which is more than enough
   for n ≤ 10 000. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*** node for a binary trie **********************************************/
struct Node
{
    int child[2];          /* indices of children, -1 if absent */
};

/*** global state (kept static across calls) **********************/
static uint8_t *fib_word      = NULL;   /* prefix of the Fibonacci word   */
static size_t   fib_word_cap  = 0;

static struct Node *nodes     = NULL; /* trie of seen phrases           */
static size_t       nodes_used = 0;
static size_t       nodes_cap  = 0;

static seq_t *cache          = NULL; /* cached phrase lengths          */
static size_t  cache_used    = 0;
static size_t  cache_cap     = 0;

static size_t pos = 0;           /* next bit to be consumed        */

/*** helper: generate prefix of Fibonacci word of length L **********/
static void generate_fib_prefix(size_t L)
{
    uint8_t *buf1 = (uint8_t*)malloc(L);
    uint8_t *buf2 = (uint8_t*)malloc(L);
    if (!buf1 || !buf2) return;     /* allocation failed – give up */

    buf1[0] = 0;
    size_t cur_len = 1;
    uint8_t *src = buf1, *dst = buf2;

    while (cur_len < L) {
        size_t new_len = 0;
        for (size_t i = 0; i < cur_len && new_len < L; ++i) {
            if (src[i] == 0) {
                if (new_len < L) dst[new_len++] = 0;
                if (new_len < L) dst[new_len++] = 1;
            } else {
                if (new_len < L) dst[new_len++] = 0;
            }
        }
        cur_len = new_len;
        /* swap source and destination */
        uint8_t *tmp = src;
        src = dst;
        dst = tmp;
    }
    /* src now holds at least L bits */
    for (size_t i = 0; i < L; ++i) fib_word[i] = src[i];

    free(buf1);
    free(buf2);
}

/*** initialise global structures on first call **********************/
static void init_global_state(void)
{
    if (!fib_word) {
        fib_word_cap = 1 << 20;              /* 1 048 576 bits – ample for n≤10 000 */
        fib_word      = (uint8_t*)malloc(fib_word_cap);
        generate_fib_prefix(fib_word_cap);
        pos = 0; /* initialise the consumption position only once */
    }
    if (!nodes) {
        nodes_cap   = 128;
        nodes       = (struct Node*)malloc(nodes_cap * sizeof(struct Node));
        nodes[0].child[0] = -1;
        nodes[0].child[1] = -1;
        nodes_used = 1;
    }
    if (!cache) {
        cache_cap   = 128;
        cache       = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_used  = 0;
    }
}

/*** parse the Fibonacci word until at least n phrases are known ********/
static void ensure_parsed_upto(idx_t n)
{
    size_t target = (size_t)n;
    while (cache_used < target) {
        size_t phrase_start = pos;     /* position where this phrase starts */
        int cur = 0;

        for (;;) {
            uint8_t bit  = fib_word[pos];
            int child    = nodes[cur].child[bit];

            if (child == -1) {          /* first unseen prefix → new phrase */
                /* grow trie if needed */
                if (nodes_used >= nodes_cap) {
                    nodes_cap = nodes_cap ? nodes_cap * 2 : 128;
                    nodes = (struct Node*)realloc(nodes,
                        nodes_cap * sizeof(struct Node));
                }
                int new_node = nodes_used;
                nodes[cur].child[bit] = new_node;
                nodes[new_node].child[0] = -1;
                nodes[new_node].child[1] = -1;
                nodes_used++;

                pos++;
                size_t phrase_len = pos - phrase_start; /* include the new bit */

                /* grow cache if needed */
                if (cache_used >= cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 128;
                    cache = (seq_t*)realloc(cache,
                        cache_cap * sizeof(seq_t));
                }
                cache[cache_used++] = (seq_t)phrase_len;
                break;
            } else {
                cur = child;
                pos++;
            }
        }
    }
}

/*** public entry point **********************************************/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n=0, return 0 */
    init_global_state();
    ensure_parsed_upto(n);
    return cache[(size_t)n - 1];
}
