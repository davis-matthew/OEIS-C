/*
 * A228736  Number of arrays of the median of three adjacent elements of some
 *          length-(n+2) 0..4 array.
 * https://oeis.org/A228736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A228736 – number of arrays of the median of three adjacent elements
 *  of some length-(n+2) 0..4 array.
 *
 *  The implementation builds the subset automaton for the underlying
 *  NFA and then counts accepted words of length n by dynamic programming.
 */

#include <stdint.h>
#include <stdlib.h>

/* -----------------------------------------------------------------------
   Constants
   ----------------------------------------------------------------------- */

#define STATE_COUNT 25          /* 5 values × 5 values for pairs (x,y)   */
#define ALPHABET    5           /* alphabet {0,1,2,3,4}                   */

/* -----------------------------------------------------------------------
   Data structures
   ----------------------------------------------------------------------- */

/* Transition information for a subset state */
struct SubState {
    uint32_t mask;            /* bitmask of included NFA states (pair indices) */
    int      trans[ALPHABET]; /* transitions on output symbols 0..4; -1 = dead */
};

/* Static data for the automaton and DP */
static struct SubState *States   = NULL; /* array of subset states          */
static size_t          StateCap  = 0;    /* capacity of States              */
static size_t          StateCnt  = 0;    /* number of created states        */

static int    *Queue     = NULL; /* for BFS over states */
static size_t QueueCap   = 0;
static size_t QueueHead  = 0;
static size_t QueueTail  = 0;

static uint32_t *NextMaskBit = NULL;          /* pre‑computed destination masks */
static uint32_t *HM_Keys    = NULL;           /* hash map keys (mask)           */
static int      *HM_Val     = NULL;           /* hash map values (state index)  */
static size_t   HM_Cap      = 0;

static seq_t  *DP_Cur   = NULL; /* DP for current length */
static seq_t  *DP_Next  = NULL; /* DP for next length   */

static seq_t  *Cache   = NULL; /* cache of sequence values a(n) */
static size_t CacheCap    = 0;
static size_t CacheSize   = 0; /* number of values stored */

/* -----------------------------------------------------------------------
   Helper functions
   ----------------------------------------------------------------------- */

/* median of three small integers (0..4) */
static inline int median3(int a, int b, int c)
{
    int mn = a < b ? a : b;
    if (c < mn) mn = c;
    int mx = a > b ? a : b;
    if (c > mx) mx = c;
    return a + b + c - mn - mx;
}

/* Simple open‑addressing hash map for mask → state index  */
static inline size_t hm_hash(uint32_t key)
{
    return (size_t)((uint64_t)key * 2654435761u) & (HM_Cap - 1);
}

static inline int hm_find(uint32_t key)
{
    size_t idx = hm_hash(key);
    for (;;) {
        if (HM_Keys[idx] == 0xFFFFFFFFu) return -1;      /* not found   */
        if (HM_Keys[idx] == key) return (int)idx;         /* found       */
        idx = (idx + 1) & (HM_Cap - 1);
    }
}

static inline void hm_insert(uint32_t key, int value)
{
    size_t idx = hm_hash(key);
    for (;;) {
        if (HM_Keys[idx] == 0xFFFFFFFFu) {
            HM_Keys[idx] = key;
            HM_Val[idx]  = value;
            return;
        }
        idx = (idx + 1) & (HM_Cap - 1);
    }
}

/* -----------------------------------------------------------------------
   State management
   ----------------------------------------------------------------------- */

/* Create or find the state corresponding to a mask. */
static int get_state_index(uint32_t mask)
{
    int idx = hm_find(mask);
    if (idx >= 0) return HM_Val[idx];

    /* New state */
    if (StateCnt == StateCap) {
        size_t newCap = StateCap ? StateCap * 2 : 256;
        struct SubState *newArr = (struct SubState *)realloc(States,
                                                             newCap * sizeof(struct SubState));
        if (!newArr) abort(); /* allocation failure */
        States = newArr;
        StateCap = newCap;
    }

    size_t pos = StateCnt++;
    States[pos].mask = mask;
    for (int c = 0; c < ALPHABET; ++c) States[pos].trans[c] = -1;

    hm_insert(mask, (int)pos);

    /* enqueue for BFS */
    if (QueueTail == QueueCap) {
        size_t newCap = QueueCap ? QueueCap * 2 : 256;
        int *newQ = (int *)realloc(Queue, newCap * sizeof(int));
        if (!newQ) abort();
        Queue = newQ;
        QueueCap = newCap;
    }
    Queue[QueueTail++] = (int)pos;

    return (int)pos;
}

/* -----------------------------------------------------------------------
   Automaton construction
   ----------------------------------------------------------------------- */

static void build_automaton(void)
{
    /* ------------------------------------------------------------
       Hash map for mask → state index
       ------------------------------------------------------------ */
    HM_Cap = 1 << 20; /* 1,048,576 entries – more than enough */
    HM_Keys = (uint32_t *)malloc(HM_Cap * sizeof(uint32_t));
    HM_Val  = (int *)malloc(HM_Cap * sizeof(int));
    for (size_t i = 0; i < HM_Cap; ++i) {
        HM_Keys[i] = 0xFFFFFFFFu; /* sentinel */
        HM_Val[i]  = -1;
    }

    /* ------------------------------------------------------------
       State array and BFS queue
       ------------------------------------------------------------ */
    StateCap = 256;
    States   = (struct SubState *)malloc(StateCap * sizeof(struct SubState));
    QueueCap = 256;
    Queue    = (int *)malloc(QueueCap * sizeof(int));

    /* ------------------------------------------------------------
       Pre‑compute transition masks for single NFA states
       NextMaskBit[median][pair] -> bitmask of destination pair states
       ------------------------------------------------------------ */
    NextMaskBit = (uint32_t *)calloc(ALPHABET * STATE_COUNT, sizeof(uint32_t));
    for (int s = 0; s < STATE_COUNT; ++s) {
        int a = s / ALPHABET;
        int b = s % ALPHABET;
        for (int z = 0; z < ALPHABET; ++z) {
            int m   = median3(a, b, z);
            int dest = b * ALPHABET + z;  /* pair (b, z) */
            NextMaskBit[m * STATE_COUNT + s] |= (1u << dest);
        }
    }

    /* ------------------------------------------------------------
       Initial state = all 25 pair states
       ------------------------------------------------------------ */
    uint32_t startMask = (1u << STATE_COUNT) - 1u;
    int startIdx = get_state_index(startMask);

    /* ------------------------------------------------------------
       BFS traversal to compute all reachable subset states
       ------------------------------------------------------------ */
    QueueHead = 0;
    QueueTail = 1;
    Queue[0] = startIdx;

    while (QueueHead < QueueTail) {
        int curIdx = Queue[QueueHead++];
        uint32_t curMask = States[curIdx].mask;

        for (int c = 0; c < ALPHABET; ++c) {
            uint32_t nextMask = 0;
            /* for every NFA state in curMask, add its destinations */
            for (int s = 0; s < STATE_COUNT; ++s) {
                if (curMask & (1u << s)) {
                    nextMask |= NextMaskBit[c * STATE_COUNT + s];
                }
            }
            if (nextMask == 0u) {
                States[curIdx].trans[c] = -1; /* dead */
            } else {
                int nxtIdx = get_state_index(nextMask);
                States[curIdx].trans[c] = nxtIdx;
            }
        }
    }

    /* ------------------------------------------------------------
       Allocate DP tables
       ------------------------------------------------------------ */
    DP_Cur  = (seq_t *)malloc(StateCnt * sizeof(seq_t));
    DP_Next = (seq_t *)malloc(StateCnt * sizeof(seq_t));
    for (size_t i = 0; i < StateCnt; ++i) DP_Cur[i] = 0;
    DP_Cur[0] = 1; /* empty word (length 0) transitions from start state */

    /* ------------------------------------------------------------
       Cache for initial values
       ------------------------------------------------------------ */
    CacheCap = 256;
    Cache = (seq_t *)malloc(CacheCap * sizeof(seq_t));
}

/* -----------------------------------------------------------------------
   Main sequence function
   ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset <1 */

    static int initialized = 0;
    if (!initialized) {
        build_automaton();
        initialized = 1;
    }

    /* Extend cached values up to n */
    while (CacheSize < (size_t)n) {
        /* zero DP_Next */
        for (size_t i = 0; i < StateCnt; ++i) DP_Next[i] = 0;

        /* transition step */
        for (size_t p = 0; p < StateCnt; ++p) {
            seq_t val = DP_Cur[p];
            if (val == 0) continue;
            for (int c = 0; c < ALPHABET; ++c) {
                int dst = States[p].trans[c];
                if (dst >= 0) {
                    DP_Next[dst] += val;
                }
            }
        }

        /* sum all DP_Next to get number of words of this length */
        seq_t total = 0;
        for (size_t s = 0; s < StateCnt; ++s) total += DP_Next[s];

        /* store in cache */
        if (CacheSize == CacheCap) {
            CacheCap *= 2;
            Cache = (seq_t *)realloc(Cache, CacheCap * sizeof(seq_t));
        }
        Cache[CacheSize] = total;
        CacheSize++;

        /* swap DP arrays */
        seq_t *tmp = DP_Cur;
        DP_Cur = DP_Next;
        DP_Next = tmp;
    }

    return Cache[n - 1];
}
